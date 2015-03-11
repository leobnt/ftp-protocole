/*
** test.c for  in /home/benoit_l/rendu/PSU_2014_myftp/test
** 
** Made by Leo BENOIT
** Login   <benoit_l@epitech.net>
** 
** Started on  Tue Mar 10 14:07:17 2015 Leo BENOIT
** Last update Tue Mar 10 15:59:45 2015 Leo BENOIT
*/

#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int	main()
{
  if (fork() == 0)
    {
      int fd = open("file", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

      dup2(fd, 1);
      dup2(fd, 2);
      write(1, "petit test\n", strlen("petit test\n"));
      close(fd);

    }
  write(1, "petit test\n", strlen("petit test\n"));
}
