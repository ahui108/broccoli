/*************************************************************************
	> File Name: hello.c
	> Author: 
	> Mail: 
	> Created Time: 2018年04月27日 星期五 09时29分55秒
 ************************************************************************/

#include <linux/init.h>         /* printk() */  
#include <linux/module.h>       /* __init __exit */  

#ifdef DEBUG  
#define PRINTK(fmt, arg...) printk(KERN_WARNING fmt, ##arg)  
#else
#define PRINTK(fmt, arg...) printk(KERN_DEBUG fmt, ##arg)  
#endif  

static char *string_test="default paramater";  
static int num_test=1000;  

/*模块加载函数，通过insmod命令加载模块时，被自动执行*/  
static int  __init  hello_init(void)      
{  
   printk(KERN_INFO " Hello World enter\n");  
   PRINTK("\nthe string_test is : %s\n",string_test); 
   PRINTK("the num_test is : %d\n",num_test);  
   return 0;  
}

/*模块卸载函数，当通过rmmod命令卸载时，会被自动执行*/
static void  __exit  hello_exit(void)    
{
    printk(KERN_INFO " Hello World exit\n ");  
}

int add_test(int a ,int b)                               
{                                  
    return a + b;                               
}   

int sub_test(int a,int b)                              
{                                  
    return a - b;                               
}       

module_init(hello_init);  
module_exit(hello_exit);  

module_param(num_test,int,S_IRUGO);
module_param(string_test,charp,S_IRUGO);

EXPORT_SYMBOL(add_test);
EXPORT_SYMBOL(sub_test);

/*模块作者，可选*/  
MODULE_AUTHOR("xxxx");
/*模块许可证明，描述内核模块的许可权限，必须*/  
MODULE_LICENSE("Dual BSD/GPL");
/*模块说明，可选*/
MODULE_DESCRIPTION("A simple Hello World Module");
/*模块说明，可选*/
MODULE_ALIAS("a simplest module");
