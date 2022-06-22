#include <stdio.h>

int main(void) {
  char 数据 = 'x';
  char *地址 = &数据;

  printf("数据=%c 地址=0x%p 地址长度=%zu字节\n", 数据, 地址, sizeof(地址));

  return 0;
}
