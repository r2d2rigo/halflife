using System.IO;
using System.Text;

namespace GoldSrc.Wad
{
    public class WadFile
    {
        private Stream _stream;
        private BinaryReader _reader;

        public WadHeader Header;
        public WadDirEntry[] DirEntries;
        public WadTexture[] Textures;

        public static WadFile Load(Stream stream)
        {
            var newWad = new WadFile();
            newWad._stream = stream;
            newWad._reader = new BinaryReader(newWad._stream);
            newWad.ReadHeader();
            newWad.ReadContents();

            return newWad;
        }

        public bool TryGetDirEntry(string name, out WadDirEntry entry)
        {
            for (int i = 0; i < Header.DirEntries; i++)
            {
                var entryNameString = CharArrayConverter.CharArrayToString(DirEntries[i].Name);
                if (entryNameString.Equals(name, System.StringComparison.OrdinalIgnoreCase))
                {
                    entry = DirEntries[i];

                    return true;
                }
            }

            entry = new WadDirEntry();

            return false;
        }

        public bool TryGetTexture(string name, out WadTexture texture)
        {
            for (int i = 0; i < Header.DirEntries; i++)
            {
                var entryNameString = CharArrayConverter.CharArrayToString(DirEntries[i].Name);
                if (entryNameString.Equals(name, System.StringComparison.OrdinalIgnoreCase))
                {
                    texture = Textures[i];

                    return true;
                }
            }

            texture = new WadTexture();

            return false;
        }

        public byte[] GetTextureData(WadDirEntry entry)
        {
            for (int i = 0; i < Header.DirEntries; i++)
            {
                var entryNameString = CharArrayConverter.CharArrayToString(entry.Name);
                var textureNameString = CharArrayConverter.CharArrayToString(Textures[i].Name);
                if (entryNameString.Equals(textureNameString, System.StringComparison.OrdinalIgnoreCase))
                {
                    var newTexture = Textures[i];

                    var firstMipSize = (int)(newTexture.Width * newTexture.Height);
                    var lastMipSize = (int)(firstMipSize * 0.25f * 0.25 * 0.25f);

                    _reader.BaseStream.Seek(entry.FilePos + newTexture.Offsets[0], SeekOrigin.Begin);
                    var textureData = _reader.ReadBytes(firstMipSize);

                    _reader.BaseStream.Seek(entry.FilePos + newTexture.Offsets[3] + lastMipSize + 2, SeekOrigin.Begin);
                    var paletteData = _reader.ReadBytes(256 * 3);

                    var textureRgbData = new byte[firstMipSize * 4];

                    for (int j = 0; j < firstMipSize; j++)
                    {
                        textureRgbData[(j * 4)] = paletteData[textureData[j] * 3];
                        textureRgbData[(j * 4) + 1] = paletteData[textureData[j] * 3 + 1];
                        textureRgbData[(j * 4) + 2] = paletteData[textureData[j] * 3 + 2];
                        textureRgbData[(j * 4) + 3] = 255;
                    }

                    return textureRgbData;
                }
            }

            return null;
        }

        private void ReadHeader()
        {
            Header = new WadHeader();

            Header.MagicNumber = _reader.ReadBytes(4);
            Header.DirEntries = _reader.ReadInt32();
            Header.DirEntriesOffset = _reader.ReadInt32();
        }

        private void ReadContents()
        {
            _reader.BaseStream.Seek(Header.DirEntriesOffset, SeekOrigin.Begin);

            DirEntries = new WadDirEntry[Header.DirEntries];

            for (int i = 0; i < Header.DirEntries; i++)
            {
                WadDirEntry newEntry = new WadDirEntry();
                newEntry.FilePos = _reader.ReadInt32();
                newEntry.DiskSize = _reader.ReadInt32();
                newEntry.Size = _reader.ReadInt32();
                newEntry.Type = _reader.ReadByte();
                newEntry.Compression = _reader.ReadBoolean();
                newEntry.Dummy = _reader.ReadInt16();
                newEntry.Name = _reader.ReadChars(16);

                DirEntries[i] = newEntry;
            }

            Textures = new WadTexture[Header.DirEntries];

            for (int i = 0; i < Header.DirEntries; i++)
            {
                _reader.BaseStream.Seek(DirEntries[i].FilePos, SeekOrigin.Begin);

                WadTexture newTexture = new WadTexture();
                newTexture.Name = _reader.ReadChars(16);
                newTexture.Width = _reader.ReadUInt32();
                newTexture.Height = _reader.ReadUInt32();
                newTexture.Offsets = new uint[4];
                newTexture.Offsets[0] = _reader.ReadUInt32();
                newTexture.Offsets[1] = _reader.ReadUInt32();
                newTexture.Offsets[2] = _reader.ReadUInt32();
                newTexture.Offsets[3] = _reader.ReadUInt32();

                Textures[i] = newTexture;
            }
        }
    }

    public static class CharArrayConverter
    {
        public static string CharArrayToString(char[] chars)
        {
            var stringBuilder = new StringBuilder();

            for (int i = 0; i < chars.Length; i++)
            {
                if (chars[i] == '\0')
                {
                    break;
                }

                stringBuilder.Append(chars[i]);
            }

            return stringBuilder.ToString();
        }
    }
}