#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    system("rfkill  unblock Bluetooth");  
  
system("bluetoothctl");  
  
system("scan on");  
  
system("pairable on");  
  
system("rfcomm connect rfcomm0");

printf("yay");
}
