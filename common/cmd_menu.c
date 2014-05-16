#include <common.h>
#include <command.h>
#include <nand.h>
#include <linux/string.h>
extern int readline (const char *const prompt);
extern char console_buffer[];

void param_menu_usage()
{
    printf("##### Parameter Set Menu #####\n");
    printf("[v] View the parameters\n");
    printf("[s] Set parameter \n");
    printf("[d] Delete parameter \n");
    printf("[w] Save your  parameters to flash memeory \n");
    printf("[q] Quit \n");
    printf("Enter your selection: ");
}

void param_menu_shell(void)
{
    char c;
    char cmd_buf[256];
    char name_buf[20];
    char val_buf[256];
   
    while (1)
    {
        param_menu_usage();
        c = getc();
        printf("%c\n", c);
        switch (c)
        {
            case 'v':
            {
                strcpy(cmd_buf, "printenv ");
                printf("Name(Hint:Press 'Enter' to view all paramters): ");
                readline(NULL);
                strcat(cmd_buf, console_buffer);
                run_command(cmd_buf, 0);
                break;
            }
            
            case 's':
            {
                sprintf(cmd_buf, "setenv ");
                printf("Name: ");
                readline(NULL);
                strcat(cmd_buf, console_buffer);
                printf("Value: ");
                readline(NULL);
                strcat(cmd_buf, " ");
                strcat(cmd_buf, console_buffer);
                run_command(cmd_buf, 0);
                break;
            }
            
            case 'd':
            {
                sprintf(cmd_buf, "setenv ");
                printf("Name: ");
                readline(NULL);
                strcat(cmd_buf, console_buffer);
                run_command(cmd_buf, 0);
                break;
            }
            
            case 'w':
            {
                sprintf(cmd_buf, "saveenv");
                run_command(cmd_buf, 0);
                break;
            }
            
            case 'q':
            {
                return;
                break;
            }
        }
    }
}

void main_menu_usage(void)
{
    printf("#####      Tiny4412 U-Boot -2010.02         #####\n");
    printf("#####      Welcome to Embeded World         #####\n");
   
    printf("[u] Download u-boot  to Nand Flash\n");
    printf("[k] Download kernel to Nand Flash\n");
    printf("[d] Download system and ramdisk to Nand Flash \n");
    printf("[s] Set the boot parameters\n");
    printf("[f] fdisk disk\n");
    printf("[t] test ramdisk\n");
    printf("[b] Boot the system\n");
    printf("[r] Reboot U-Boot\n");
    printf("[q] Quit from menu\n");
    printf("Enter your selection: ");
}
void menu_shell(void)
{
    char c;
    char cmd_buf[256];
    while (1)
    {
        main_menu_usage();
        c = getc();
        printf("%c\n", c);
        switch (c)
        {
			case 'u':
			case 'U':
			{
				memset(cmd_buf,0,sizeof(cmd_buf));
				strcpy(cmd_buf,"emmc open 1;movi read fwbl1 0 40000000;movi w z f 1 40000000;");
				run_command(cmd_buf,0);
				memset(cmd_buf,0,sizeof(cmd_buf));
				strcpy(cmd_buf,"movi read bl2 0 40000000; movi w z b 1 40000000;");
				run_command(cmd_buf,0);
				memset(cmd_buf,0,sizeof(cmd_buf));
				strcpy(cmd_buf,"movi read u-boot 0 40000000;movi w z u 1 40000000;");
				run_command(cmd_buf,0);
				memset(cmd_buf,0,sizeof(cmd_buf));
				strcpy(cmd_buf,"movi read tzsw 0 40000000;movi w z t 1 40000000;emmc close 1;");
				run_command(cmd_buf,0);
				break;
			}
			case 'k':
			case 'K':
			{
				memset(cmd_buf,0,sizeof(cmd_buf));
				strcpy(cmd_buf,"fatload mmc 0:1 0x40008000 zImage ;movi write kernel 1 0x40008000;");
				run_command(cmd_buf,0);
			
				break;
			}
			case 'd':
			case 'D':
			{
				memset(cmd_buf,0,sizeof(cmd_buf));
				strcpy(cmd_buf,"fatload mmc 0:1 0x41000000 ramdisk-4.2.2.img ;movi w r 1 0x41000000 100000;");
				run_command(cmd_buf,0);
			//	memset(cmd_buf,0,sizeof(cmd_buf));
			//	strcpy(cmd_buf,"fatload mmc 0:1 0x42000000 system.img ;fastboot flash system 0x42000000;");
			//	run_command(cmd_buf,0);
				break;
			}
			case 's':
			case 'S':
			{
				param_menu_shell();
				break;
			}
			case 'b':
        	case 'B':
			{
				memset(cmd_buf,0,sizeof(cmd_buf));
				strcpy(cmd_buf,"movi read kernel 0 40008000;movi read rootfs 0 41000000 100000;bootm 40008000 41000000");
				run_command(cmd_buf,0);
				break;
			}
        	case 'r':
        	case 'R':
        	{
				memset(cmd_buf,0,sizeof(cmd_buf));
				strcpy(cmd_buf,"reset");
				run_command(cmd_buf,0);
				break;
			}
			case 'f':
			case 'F':
			{
				memset(cmd_buf,0,sizeof(cmd_buf));
				strcpy(cmd_buf,"fdisk -c 1 320 806 518;fatformat mmc 1:1;");
				run_command(cmd_buf,0);
				break;
			}
			case 't':
			case 'T':
			{
				char *p = (char*)0x43000000;
				char *dst;
				memset(cmd_buf,0,sizeof(cmd_buf));
				strcpy(cmd_buf,"fatload mmc 0:1 0x43000000 tiny4412.ini;");
				run_command(cmd_buf,0);
				printf("%s\n",p);
				dst = strstr(p,"version=4.2.2");
				if(dst)
				{
					printf("%s\n",dst);
				}
				
				break;
			}
        	case 'Q':
            case 'q':
            {
            	printf("\n");
                return;   
                break;
            }
        }
               
    }
}
int do_menu (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    menu_shell();
    return 0;
}
U_BOOT_CMD(
menu, 3, 0, do_menu,
"menu - display a menu, to select the items to do something/n",
" - display a menu, to select the items to do something"
);

