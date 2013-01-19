using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using MahApps.Metro.Controls;
using System.Diagnostics;

namespace Gui
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : MetroWindow
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            String AppPath = @"Yandere.exe ";
            //System.Diagnostics.Process.Start(tagsparameter);
            Process Proceso = new Process();
            Proceso.StartInfo.FileName = AppPath;
            Proceso.StartInfo.Arguments = tags.Text;
            if (tags.Text=="")
            {
                MessageBox.Show("You have to put at least 1 tag");
            } 
            else
            {
                Proceso.Start();
            }
            //MessageBox.Show(tagsparameter);
        }
        public void TextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            tb.Text = string.Empty;
            tb.GotFocus -= TextBox_GotFocus;
        }
    }
}
