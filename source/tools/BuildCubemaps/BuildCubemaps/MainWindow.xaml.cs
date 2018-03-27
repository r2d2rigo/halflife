using GoldSrc.Bsp;
using GoldSrc.Wad;
using OpenTK;
using OpenTK.Graphics;
using OpenTK.Graphics.OpenGL;
using System;
using System.Collections.Generic;
using System.IO;
using System.Threading;
using System.Windows;
using System.Windows.Forms;
using System.Windows.Forms.Integration;
using System.Windows.Media.Imaging;

namespace BuildCubemaps
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private const int DEFAULT_CUBEMAP_SIZE = 32;
        private static readonly Tuple<string, Vector3, Vector3, float>[] CUBEMAP_FACES =
            {
            new Tuple<string, Vector3, Vector3, float>("lf", -Vector3.UnitX, Vector3.UnitY, -90),
            new Tuple<string, Vector3, Vector3, float>("rt", Vector3.UnitX, Vector3.UnitY, 90),
            new Tuple<string, Vector3, Vector3, float>("ft", -Vector3.UnitZ, Vector3.UnitY, 180),
            new Tuple<string, Vector3, Vector3, float>("bk", Vector3.UnitZ, Vector3.UnitY, 0),
            new Tuple<string, Vector3, Vector3, float>("dn", -Vector3.UnitY, -Vector3.UnitZ, 180),
            new Tuple<string, Vector3, Vector3, float>("up", Vector3.UnitY, Vector3.UnitZ, 0),
            };

        private GLControl _glControl;
        private BspFile _bsp;
        private WadFile _wad;
        private Dictionary<string, int> _textures;
        private Dictionary<int, int> _lightmaps;
        private bool _isInitialized;
        private ManualResetEvent _resetEvent;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void WindowsFormsHost_Initialized(object sender, EventArgs e)
        {
            if (App.Arguments == null)
            {
                return;
            }

            _bsp = BspFile.Load(File.OpenRead(App.Arguments.InputFile));
            _wad = WadFile.Load(File.OpenRead(Path.Combine(App.Arguments.WadDirectory, "halflife.wad")));
            _textures = new Dictionary<string, int>();
            _lightmaps = new Dictionary<int, int>();

            _resetEvent = new ManualResetEvent(false);
            _glControl = new GLControl(new GraphicsMode(32, 24), 2, 0, GraphicsContextFlags.Default);
            _glControl.MakeCurrent();
            _glControl.Paint += GLControl_Paint;
            _glControl.Dock = DockStyle.Fill;

            (sender as WindowsFormsHost).Child = _glControl;
        }

        private void GLControl_Paint(object sender, PaintEventArgs e)
        {
            if (!_isInitialized)
            {
                GL.Enable(EnableCap.Texture2D);

                foreach (var mipTexture in _bsp.MipTextures)
                {
                    WadDirEntry entry = new WadDirEntry();
                    WadTexture texture = new WadTexture();

                    _wad.TryGetDirEntry(CharArrayConverter.CharArrayToString(mipTexture.Name), out entry);
                    _wad.TryGetTexture(CharArrayConverter.CharArrayToString(mipTexture.Name), out texture);
                    var textureData = _wad.GetTextureData(entry);

                    var textureIndex = GL.GenTexture();
                    GL.BindTexture(TextureTarget.Texture2D, textureIndex);
                    GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.Rgba, (int)texture.Width, (int)texture.Height, 0, PixelFormat.Rgba, PixelType.UnsignedByte, textureData);
                    GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)TextureMagFilter.Linear);
                    GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMagFilter.Linear);

                    _textures.Add(CharArrayConverter.CharArrayToString(mipTexture.Name), textureIndex);
                }

                GenerateLightmaps();

                _isInitialized = true;
            }

            if (App.Arguments.AmbientLighting)
            {
                foreach (var leafAmbientLighting in _bsp.LeafAmbientLights)
                {
                    var cubemapOrigin = new Vector3(leafAmbientLighting.Position[0], leafAmbientLighting.Position[1], leafAmbientLighting.Position[2]);
                    var cubemapName = $"{(int)cubemapOrigin.X}_{(int)cubemapOrigin.Y}_{(int)cubemapOrigin.Z}";

                    var cubemapSize = 1;
                    int faceIndex = 0;

                    foreach (var faceInfo in CUBEMAP_FACES)
                    {
                        var pixelData = RenderCubemap(cubemapOrigin, cubemapOrigin + faceInfo.Item2, faceInfo.Item3, cubemapName + faceInfo.Item1, faceInfo.Item4, cubemapSize, true);
                        leafAmbientLighting.AmbientColor[faceIndex * 3] = pixelData[0];
                        leafAmbientLighting.AmbientColor[faceIndex * 3 + 1] = pixelData[1];
                        leafAmbientLighting.AmbientColor[faceIndex * 3 + 2] = pixelData[2];

                        faceIndex++;
                    }
                }
            }
            else
            {
                foreach (var cubemap in _bsp.Cubemaps)
                {
                    var cubemapOrigin = new Vector3(cubemap.Position[0], cubemap.Position[1], cubemap.Position[2]);
                    var cubemapName = $"{(int)cubemapOrigin.X}_{(int)cubemapOrigin.Y}_{(int)cubemapOrigin.Z}";

                    var cubemapSize = DEFAULT_CUBEMAP_SIZE;

                    if (cubemap.Size > 0)
                    {
                        cubemapSize = (int)Math.Pow(2, cubemap.Size - 1);
                    }

                    var cubemapData = new List<byte>();

                    foreach (var faceInfo in CUBEMAP_FACES)
                    {
                        cubemapData.AddRange(RenderCubemap(cubemapOrigin, cubemapOrigin + faceInfo.Item2, faceInfo.Item3, cubemapName + faceInfo.Item1, faceInfo.Item4, cubemapSize, false));
                    }

                    cubemap.Data = cubemapData.ToArray();
                }
            }

            _bsp.Save(File.Create(App.Arguments.OutputFile));

            App.Current.Shutdown();
        }

        private byte[] RenderCubemap(Vector3 cameraPosition, Vector3 cameraTarget, Vector3 cameraUp, string faceName, float faceRotation, int cubemapSize, bool lightmapOnly)
        {
            GL.DepthMask(true);
            GL.Enable(EnableCap.DepthTest);

            GL.ClearColor(74.0f / 255.0f, 148.0f / 255.0f, 255.0f / 255.0f, 1.0f);
            GL.Clear(ClearBufferMask.ColorBufferBit |
                     ClearBufferMask.DepthBufferBit |
                     ClearBufferMask.StencilBufferBit);

            GL.Viewport(0, 0, cubemapSize, cubemapSize);

            Matrix4 projection = Matrix4.CreatePerspectiveFieldOfView(90.0f * (float)(Math.PI / 180.0f), cubemapSize / (float)cubemapSize, 1.0f, 1000.0f);
            GL.MatrixMode(MatrixMode.Projection);
            GL.LoadMatrix(ref projection);

            Matrix4 modelview = Matrix4.LookAt(cameraPosition, cameraTarget, cameraUp);
            GL.MatrixMode(MatrixMode.Modelview);
            GL.LoadMatrix(ref modelview);

            GL.Color3(1.0f, 1.0f, 1.0f);

            foreach (var leaf in _bsp.Leaves)
            {
                for (int i = leaf.FirstMarkSurface; i < leaf.FirstMarkSurface + leaf.MarkSurfaceCount; i++)
                {
                    var faceIndex = (int)_bsp.MarkSurfaces[i];
                    var currentFace = _bsp.Faces[faceIndex];
                    var faceTriangleCount = currentFace.EdgeCount - 2;
                    var facePlane = _bsp.Planes[currentFace.Plane];
                    var faceTextureInfo = _bsp.TexInfos[currentFace.TextureInfo];
                    var faceTexture = _bsp.MipTextures[faceTextureInfo.MipTex];

                    if (faceTextureInfo.Flags != 0)
                        continue;

                    if (lightmapOnly)
                    {
                        GL.ActiveTexture(TextureUnit.Texture0);
                        GL.Enable(EnableCap.Texture2D);
                        GL.BindTexture(TextureTarget.Texture2D, _lightmaps[(int)currentFace.LightmapOffset]);
                    }
                    else
                    {
                        GL.ActiveTexture(TextureUnit.Texture0);
                        GL.Enable(EnableCap.Texture2D);
                        GL.BindTexture(TextureTarget.Texture2D, _textures[CharArrayConverter.CharArrayToString(faceTexture.Name)]);

                        GL.ActiveTexture(TextureUnit.Texture1);
                        GL.Enable(EnableCap.Texture2D);
                        GL.BindTexture(TextureTarget.Texture2D, _lightmaps[(int)currentFace.LightmapOffset]);
                    }

                    GL.Begin(PrimitiveType.Triangles);

                    var minU = int.MaxValue;
                    var minV = int.MaxValue;
                    var maxU = int.MinValue;
                    var maxV = int.MinValue;

                    for (int j = 0; j < faceTriangleCount; j++)
                    {
                        var vertexIndex1 = GetSurfEdgeVertex(_bsp, (int)_bsp.SurfEdges[currentFace.FirstEdge]);
                        var vertexIndex2 = GetSurfEdgeVertex(_bsp, (int)_bsp.SurfEdges[currentFace.FirstEdge + j + 1]);
                        var vertexIndex3 = GetSurfEdgeVertex(_bsp, (int)_bsp.SurfEdges[currentFace.FirstEdge + j + 2]);

                        var vertex1 = _bsp.Vertices[vertexIndex1];
                        var vertex2 = _bsp.Vertices[vertexIndex2];
                        var vertex3 = _bsp.Vertices[vertexIndex3];

                        var texCoord1 = new Vector2((Vector3.Dot(vertex1, faceTextureInfo.S) + faceTextureInfo.SShift), (Vector3.Dot(vertex1, faceTextureInfo.T) + faceTextureInfo.TShift));
                        var texCoord2 = new Vector2((Vector3.Dot(vertex2, faceTextureInfo.S) + faceTextureInfo.SShift), (Vector3.Dot(vertex2, faceTextureInfo.T) + faceTextureInfo.TShift));
                        var texCoord3 = new Vector2((Vector3.Dot(vertex3, faceTextureInfo.S) + faceTextureInfo.SShift), (Vector3.Dot(vertex3, faceTextureInfo.T) + faceTextureInfo.TShift));

                        if (Math.Floor(texCoord1.X) < minU) { minU = (int)Math.Floor(texCoord1.X); }
                        if (Math.Floor(texCoord1.Y) < minV) { minV = (int)Math.Floor(texCoord1.Y); }
                        if (Math.Floor(texCoord1.X) > maxU) { maxU = (int)Math.Floor(texCoord1.X); }
                        if (Math.Floor(texCoord1.Y) > maxV) { maxV = (int)Math.Floor(texCoord1.Y); }

                        if (Math.Floor(texCoord2.X) < minU) { minU = (int)Math.Floor(texCoord2.X); }
                        if (Math.Floor(texCoord2.Y) < minV) { minV = (int)Math.Floor(texCoord2.Y); }
                        if (Math.Floor(texCoord2.X) > maxU) { maxU = (int)Math.Floor(texCoord2.X); }
                        if (Math.Floor(texCoord2.Y) > maxV) { maxV = (int)Math.Floor(texCoord2.Y); }

                        if (Math.Floor(texCoord3.X) < minU) { minU = (int)Math.Floor(texCoord3.X); }
                        if (Math.Floor(texCoord3.Y) < minV) { minV = (int)Math.Floor(texCoord3.Y); }
                        if (Math.Floor(texCoord3.X) > maxU) { maxU = (int)Math.Floor(texCoord3.X); }
                        if (Math.Floor(texCoord3.Y) > maxV) { maxV = (int)Math.Floor(texCoord3.Y); }
                    }

                    var lightmapWidth = (int)(Math.Ceiling(maxU / 16.0f) - Math.Floor(minU / 16.0f) + 1);
                    var lightmapHeight = (int)(Math.Ceiling(maxV / 16.0f) - Math.Floor(minV / 16.0f) + 1);

                    for (int j = 0; j < faceTriangleCount; j++)
                    {
                        // TODO: unique vertices!
                        var vertexIndex1 = GetSurfEdgeVertex(_bsp, (int)_bsp.SurfEdges[currentFace.FirstEdge]);
                        var vertexIndex2 = GetSurfEdgeVertex(_bsp, (int)_bsp.SurfEdges[currentFace.FirstEdge + j + 1]);
                        var vertexIndex3 = GetSurfEdgeVertex(_bsp, (int)_bsp.SurfEdges[currentFace.FirstEdge + j + 2]);

                        var vertex1 = _bsp.Vertices[vertexIndex1];
                        var vertex2 = _bsp.Vertices[vertexIndex2];
                        var vertex3 = _bsp.Vertices[vertexIndex3];

                        var normal = currentFace.PlaneSide == 0 ? facePlane.Normal : facePlane.Normal * -1;

                        var v1tx = new Vector2(((Vector3.Dot(vertex1, faceTextureInfo.S) + faceTextureInfo.SShift) / faceTexture.Width), ((Vector3.Dot(vertex1, faceTextureInfo.T) + faceTextureInfo.TShift) / faceTexture.Height));
                        var v1lmtx = new Vector2((Vector3.Dot(vertex1, faceTextureInfo.S) + faceTextureInfo.SShift - minU + 8) / (lightmapWidth * 16.0f), (Vector3.Dot(vertex1, faceTextureInfo.T) + faceTextureInfo.TShift - minV + 8) / (lightmapHeight * 16.0f));

                        if (lightmapOnly)
                        {
                            GL.MultiTexCoord2(TextureUnit.Texture0, v1lmtx.X, v1lmtx.Y);
                            GL.Vertex3(vertex1.X, vertex1.Y, vertex1.Z);
                        }
                        else
                        {
                            GL.MultiTexCoord2(TextureUnit.Texture0, v1tx.X, v1tx.Y);
                            GL.MultiTexCoord2(TextureUnit.Texture1, v1lmtx.X, v1lmtx.Y);
                            GL.Vertex3(vertex1.X, vertex1.Y, vertex1.Z);
                        }

                        var v2tx = new Vector2(((Vector3.Dot(vertex2, faceTextureInfo.S) + faceTextureInfo.SShift) / faceTexture.Width), ((Vector3.Dot(vertex2, faceTextureInfo.T) + faceTextureInfo.TShift) / faceTexture.Height));
                        var v2lmtx = new Vector2((Vector3.Dot(vertex2, faceTextureInfo.S) + faceTextureInfo.SShift - minU + 8) / (lightmapWidth * 16.0f), (Vector3.Dot(vertex2, faceTextureInfo.T) + faceTextureInfo.TShift - minV + 8) / (lightmapHeight * 16.0f));

                        if (lightmapOnly)
                        {
                            GL.MultiTexCoord2(TextureUnit.Texture0, v2lmtx.X, v2lmtx.Y);
                            GL.Vertex3(vertex2.X, vertex2.Y, vertex2.Z);
                        }
                        else
                        {
                            GL.MultiTexCoord2(TextureUnit.Texture0, v2tx.X, v2tx.Y);
                            GL.MultiTexCoord2(TextureUnit.Texture1, v2lmtx.X, v2lmtx.Y);
                            GL.Vertex3(vertex2.X, vertex2.Y, vertex2.Z);
                        }

                        var v3tx = new Vector2(((Vector3.Dot(vertex3, faceTextureInfo.S) + faceTextureInfo.SShift) / faceTexture.Width), ((Vector3.Dot(vertex3, faceTextureInfo.T) + faceTextureInfo.TShift) / faceTexture.Height));
                        var v3lmtx = new Vector2((Vector3.Dot(vertex3, faceTextureInfo.S) + faceTextureInfo.SShift - minU + 8) / (lightmapWidth * 16.0f), (Vector3.Dot(vertex3, faceTextureInfo.T) + faceTextureInfo.TShift - minV + 8) / (lightmapHeight * 16.0f));

                        if (lightmapOnly)
                        {
                            GL.MultiTexCoord2(TextureUnit.Texture0, v3lmtx.X, v3lmtx.Y);
                            GL.Vertex3(vertex3.X, vertex3.Y, vertex3.Z);
                        }
                        else
                        {
                            GL.MultiTexCoord2(TextureUnit.Texture0, v3tx.X, v3tx.Y);
                            GL.MultiTexCoord2(TextureUnit.Texture1, v3lmtx.X, v3lmtx.Y);
                            GL.Vertex3(vertex3.X, vertex3.Y, vertex3.Z);
                        }
                    }

                    GL.End();
                }
            }

            GL.DepthMask(false);
            GL.Flush();

            var pixels = new byte[cubemapSize * cubemapSize * 3];
            GL.ReadPixels<byte>(0, 0, cubemapSize, cubemapSize, PixelFormat.Rgb, PixelType.UnsignedByte, pixels);

            var finalTransform = new System.Windows.Media.TransformGroup();
            finalTransform.Children.Add(new System.Windows.Media.RotateTransform(faceRotation));

            var bitmapSource = BitmapFrame.Create(cubemapSize, cubemapSize, 96.0f, 96.0f, System.Windows.Media.PixelFormats.Rgb24, null, pixels, cubemapSize * 3);
            var transformedBitmap = new TransformedBitmap(bitmapSource, finalTransform);

            var pixelData = new byte[cubemapSize * cubemapSize * 3];
            transformedBitmap.CopyPixels(pixelData, cubemapSize * 3, 0);

            /*
            var pngEncoder = new PngBitmapEncoder();
            pngEncoder.Frames.Add(BitmapFrame.Create(transformedBitmap));
            using (var outputStream = File.Create($"cubemaps/{faceName}.png"))
            {
                pngEncoder.Save(outputStream);
            }
            */
            

            _glControl.SwapBuffers();

            return pixelData;
        }

        private static Int32 GetSurfEdgeVertex(BspFile bsp, Int32 surfEdge)
        {
            if (surfEdge < 0)
            {
                return bsp.Edges[-surfEdge].Vertices[1];
            }

            return bsp.Edges[surfEdge].Vertices[0];
        }

        private void GenerateLightmaps()
        {
            byte[] lightmapData = new byte[1024 * 1024 * 3];

            foreach (var currentFace in _bsp.Faces)
            {
                var faceTriangleCount = currentFace.EdgeCount - 2;
                var faceTextureInfo = _bsp.TexInfos[currentFace.TextureInfo];
                var faceTexture = _bsp.MipTextures[faceTextureInfo.MipTex];

                if (faceTextureInfo.Flags != 0)
                    continue;

                var minU = int.MaxValue;
                var minV = int.MaxValue;
                var maxU = int.MinValue;
                var maxV = int.MinValue;

                for (int j = 0; j < faceTriangleCount; j++)
                {
                    var vertexIndex1 = GetSurfEdgeVertex(_bsp, (int)_bsp.SurfEdges[currentFace.FirstEdge]);
                    var vertexIndex2 = GetSurfEdgeVertex(_bsp, (int)_bsp.SurfEdges[currentFace.FirstEdge + j + 1]);
                    var vertexIndex3 = GetSurfEdgeVertex(_bsp, (int)_bsp.SurfEdges[currentFace.FirstEdge + j + 2]);

                    var vertex1 = _bsp.Vertices[vertexIndex1];
                    var vertex2 = _bsp.Vertices[vertexIndex2];
                    var vertex3 = _bsp.Vertices[vertexIndex3];

                    var texCoord1 = new Vector2((Vector3.Dot(vertex1, faceTextureInfo.S) + faceTextureInfo.SShift), (Vector3.Dot(vertex1, faceTextureInfo.T) + faceTextureInfo.TShift));
                    var texCoord2 = new Vector2((Vector3.Dot(vertex2, faceTextureInfo.S) + faceTextureInfo.SShift), (Vector3.Dot(vertex2, faceTextureInfo.T) + faceTextureInfo.TShift));
                    var texCoord3 = new Vector2((Vector3.Dot(vertex3, faceTextureInfo.S) + faceTextureInfo.SShift), (Vector3.Dot(vertex3, faceTextureInfo.T) + faceTextureInfo.TShift));

                    if (Math.Floor(texCoord1.X) < minU) { minU = (int)Math.Floor(texCoord1.X); }
                    if (Math.Floor(texCoord1.Y) < minV) { minV = (int)Math.Floor(texCoord1.Y); }
                    if (Math.Floor(texCoord1.X) > maxU) { maxU = (int)Math.Floor(texCoord1.X); }
                    if (Math.Floor(texCoord1.Y) > maxV) { maxV = (int)Math.Floor(texCoord1.Y); }

                    if (Math.Floor(texCoord2.X) < minU) { minU = (int)Math.Floor(texCoord2.X); }
                    if (Math.Floor(texCoord2.Y) < minV) { minV = (int)Math.Floor(texCoord2.Y); }
                    if (Math.Floor(texCoord2.X) > maxU) { maxU = (int)Math.Floor(texCoord2.X); }
                    if (Math.Floor(texCoord2.Y) > maxV) { maxV = (int)Math.Floor(texCoord2.Y); }

                    if (Math.Floor(texCoord3.X) < minU) { minU = (int)Math.Floor(texCoord3.X); }
                    if (Math.Floor(texCoord3.Y) < minV) { minV = (int)Math.Floor(texCoord3.Y); }
                    if (Math.Floor(texCoord3.X) > maxU) { maxU = (int)Math.Floor(texCoord3.X); }
                    if (Math.Floor(texCoord3.Y) > maxV) { maxV = (int)Math.Floor(texCoord3.Y); }
                }

                var blockWidth = (int)(Math.Ceiling(maxU / 16.0f) - Math.Floor(minU / 16.0f) + 1);
                var blockHeight = (int)(Math.Ceiling(maxV / 16.0f) - Math.Floor(minV / 16.0f) + 1);

                var blockData = new byte[blockWidth * blockHeight * 4];

                for (int i = 0; i < blockWidth * blockHeight; i++)
                {
                    blockData[i * 4] = _bsp.LightmapData.Data[currentFace.LightmapOffset + i * 3];
                    blockData[i * 4 + 1] = _bsp.LightmapData.Data[currentFace.LightmapOffset + i * 3 + 1];
                    blockData[i * 4 + 2] = _bsp.LightmapData.Data[currentFace.LightmapOffset + i * 3 + 2];
                    blockData[i * 4 + 3] = 0xFF;
                }

                var newLightmapIndex = GL.GenTexture();
                GL.BindTexture(TextureTarget.Texture2D, newLightmapIndex);
                GL.TexImage2D<byte>(TextureTarget.Texture2D, 0, PixelInternalFormat.Rgba, blockWidth, blockHeight, 0, PixelFormat.Rgba, PixelType.UnsignedByte, blockData);

                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapS, (int)TextureWrapMode.Clamp);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapT, (int)TextureWrapMode.Clamp);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)TextureMagFilter.Linear);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMagFilter.Linear);

                /*
                var bitmapSource = BitmapFrame.Create(blockWidth, blockHeight, 96.0f, 96.0f, System.Windows.Media.PixelFormats.Rgb24, null, blockData, blockWidth * 3);

                var pngEncoder = new PngBitmapEncoder();
                pngEncoder.Frames.Add(BitmapFrame.Create(bitmapSource));
                using (var outputStream = File.Create($"lightmaps/{currentFace.LightmapOffset.ToString()}.png"))
                {
                    pngEncoder.Save(outputStream);
                }
                */

                _lightmaps[(int)currentFace.LightmapOffset] = newLightmapIndex;
            }
        }
    }
}
