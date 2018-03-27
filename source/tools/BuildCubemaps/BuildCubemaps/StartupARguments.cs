using CommandLine;

namespace BuildCubemaps
{
    public class StartupArguments
    {
        [Option(Required = true)]
        public string InputFile { get; set; }

        [Option(Required = true)]
        public string OutputFile { get; set; }

        [Option(Required = true)]
        public string WadDirectory { get; set; }

        [Option(Required = false, Default = false)]
        public bool AmbientLighting { get; set; }
    }
}
