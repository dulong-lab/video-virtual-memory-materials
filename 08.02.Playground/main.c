#include <stdio.h>
#include <excpt.h>
#include <stdbool.h>

int
main()
{
    int Robot = 123;
    int* Pointer;

    printf("Robot @ %p\n\n", &Robot);

    while (true)
    {
        __try
        {
            printf("请输入指针>> ");
            scanf_s("%p", &Pointer);

            printf("当前值：%d\n\n", *Pointer);

            printf("请输入新值>> ");
            scanf_s("%d", Pointer);

            printf("当前值：%d\n\n\n", *Pointer);
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            printf("代码异常\n\n\n");
        }
    }
}
