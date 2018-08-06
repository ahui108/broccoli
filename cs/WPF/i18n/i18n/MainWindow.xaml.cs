using System;
using System.Collections.Generic;
using System.Globalization;
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

namespace i18n
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        public class CategoryInfo
        {
            public string Name
            {
                get;
                set;
            }
            public string Value
            {
                get;
                set;
            }

        }

        // switch language
        private void btnChangeLang_Click(object sender, RoutedEventArgs e)
        {
            object selectedName = cbLang.SelectedValue;
            if (selectedName != null)
            {
                string langName = selectedName.ToString();
                // if english, change the resource name to 'DefaultLanguage'
                if (langName == "en_US")
                    langName = "DefaultLanguage";
                
                // get locale
                // CultureInfo curCultureInfo = CultureInfo.CurrentCulture;
                // langName = curCultureInfo.Name;
                // string UILangName = CultureInfo.CurrentUICulture.Name;

                ResourceDictionary langRd = null;
                try
                {
                    langRd = Application.LoadComponent(new Uri(@"lang\" + langName + ".xaml", UriKind.Relative)) as ResourceDictionary;
                }
                catch (Exception e2)
                {
                    MessageBox.Show(e2.Message);
                }

                if (langRd != null)
                {
                    if (this.Resources.MergedDictionaries.Count > 0)
                    {
                        this.Resources.MergedDictionaries.Clear();
                    }
                    this.Resources.MergedDictionaries.Add(langRd);
                }
            }
            else
                MessageBox.Show("Please selected one Language first.");
        }

        private void cbLang_Loaded(object sender, RoutedEventArgs e)
        {
            List<CategoryInfo> categoryList = new List<CategoryInfo>();
            categoryList.Add(new CategoryInfo() { Name = "English", Value = "en_US" });
            categoryList.Add(new CategoryInfo() { Name = "中文", Value = "zh_CN" });
            cbLang.ItemsSource = categoryList;
            cbLang.DisplayMemberPath = "Name";
            cbLang.SelectedValuePath = "Value";
            cbLang.SelectedValue = "zh_CN";
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
