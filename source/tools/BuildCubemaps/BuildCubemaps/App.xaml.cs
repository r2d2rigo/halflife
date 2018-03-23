using CommandLine;
using System.Text;
using System.Windows;

namespace BuildCubemaps
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        private static StartupArguments _arguments;

        public static StartupArguments Arguments { get { return _arguments; } }

        public App()
        {
        }

        protected override void OnStartup(StartupEventArgs e)
        {
            var parseResult = Parser.Default.ParseArguments<StartupArguments>(e.Args);

            parseResult.WithNotParsed((errors) =>
            {
                var errorString = new StringBuilder();
                errorString.AppendLine("Invalid arguments.");

                MessageBox.Show(errorString.ToString());

                App.Current.Shutdown();
            });

            parseResult.WithParsed((args) =>
            {
                _arguments = args;
            });

            base.OnStartup(e);
        }
    }
}
