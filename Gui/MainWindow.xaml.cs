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
        public void StartApp(String AppPath)
        {
            Process Proceso = new Process();
            Proceso.StartInfo.FileName = AppPath;
            Proceso.StartInfo.Arguments = tags.Text;
            if (tags.Text == "")
            {
                MessageBox.Show("You have to put at least 1 tag");
            }
            else
            {
                Proceso.Start();
            }
        }
        private void Yandere(object sender, RoutedEventArgs e)
        {
            String AppPath = @"Yandere.exe ";
            StartApp(AppPath);
            //MessageBox.Show(tagsparameter);
        }
        private void Danbooru(object sender, RoutedEventArgs e)
        {
            String AppPath = @"Danbooru.exe ";
            StartApp(AppPath);
            //MessageBox.Show(tagsparameter);
        }
        private void Konachan(object sender, RoutedEventArgs e)
        {
            String AppPath = @"Konachan.exe ";
            StartApp(AppPath);
            //MessageBox.Show(tagsparameter);
        }
        private void _3dbooru(object sender, RoutedEventArgs e)
        {
            String AppPath = @"3dbooru.exe ";
            StartApp(AppPath);
            //MessageBox.Show(tagsparameter);
        }
        private void vectorbooru(object sender, RoutedEventArgs e)
        {
            String AppPath = @"vectorbooru.exe ";
            StartApp(AppPath);
            //MessageBox.Show(tagsparameter);
        }
        private void chan_at_sc(object sender, RoutedEventArgs e)
        {
            String AppPath = @"chan@sc.exe ";
            StartApp(AppPath);
            //MessageBox.Show(tagsparameter);
        }
        private void idol_at_sc(object sender, RoutedEventArgs e)
        {
            String AppPath = @"idol@sc.exe ";
            StartApp(AppPath);
            //MessageBox.Show(tagsparameter);
        }
        private void gelbooru(object sender, RoutedEventArgs e)
        {
            String AppPath = @"gelbooru.exe ";
            StartApp(AppPath);
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
