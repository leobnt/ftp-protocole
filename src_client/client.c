/*
** client.c for  in /home/benoit_l/rendu/PSU_2014_myftp
** 
** Made by Leo BENOIT
** Login   <benoit_l@epitech.net>
** 
** Started on  Tue Mar 10 16:11:41 2015 Leo BENOIT
** Last update Wed Mar 11 14:20:03 2015 Leo BENOIT
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>

int     my_strlen(char *str)
{
  int   i;

  i = 0;
  while (str[i])
    ++i;
  return (i);
}

void    my_put_error(char *str)
{
  write(2, str, my_strlen(str));
}

void    my_putstr(char *str)
{
  write(1, str, my_strlen(str));
}

void	do_client(int cs, char *str)
{
  write(cs, str, strlen(str));
}

void	my_putchar(char x)
{
  write(1, &x, 1);
}

void    my_put_nbr(int nb)
{
  if (nb < 0)
    {
      my_putchar('-');
      nb = -nb;
    }
  if (nb >= 10)
    {
      my_put_nbr(nb / 10);
      my_put_nbr(nb % 10);
    }
  else
    my_putchar(nb + '0');
}

void    clean_buff(char *buff)
{
  int   i;

  i = 0;
  while (i < 4096)
    {
      buff[i] = '\0';
      ++i;
    }
}

int     check_arg(int ac)
{
  if (ac != 3)
    {
      my_putstr("Usage: ./client [IP] [PORT]\n");
      return (-1);
    }
  return (0);
}

int     init_socket(char *nb_port)
{
  struct protoent       *pe;
  int                   port;
  int                   s;

  port = atoi(nb_port);
  if (port == 0)
    return (-1);
  pe = getprotobyname("TCP");
  s = socket(AF_INET, SOCK_STREAM, pe->p_proto);
  if (s == -1)
    {
      my_put_error("Error : Init socket\n");
      close(s);
      return (-1);
    }
  return (s);
}

int     init_connect(int s, struct sockaddr_in sin)
{
  int   error;

  error = connect(s, (const struct sockaddr *)&sin, sizeof(sin));
  if (error == -1)
    {
      my_put_error("Error: Init connect\n");
      close(s);
      return (-1);
    }
  return (0);
}

struct sockaddr_in      init_sin_struct(char *ip, int port)
{
  struct sockaddr_in    sin;

  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = inet_addr(ip);
  return (sin);
}

int     creat_connection(int s, char *ip, int port)
{
  struct sockaddr_in    sin;

  sin = init_sin_struct(ip, port);
  if (init_connect(s, sin) == -1)
    return (-1);
  return (0);
}

int	go_client(int s)
{
  char *buff;
  int   ligne = 0;

  while (42)
    {
      my_putstr("connect: benoit_l@pc ");
      my_put_nbr(ligne);
      my_putstr(" > ");
      buff = malloc(sizeof(char) * 500);
      if (read(0, buff, 500) == -1)
        {
          close(s);
          return (0);
        }
      if (strcmp(buff, "exit\n") == 0)
        {
          do_client(s, buff);
          close(s);
          return (0);
        }
      do_client(s, buff);
      clean_buff(buff);
      if (read(s, buff, 500) == -1)
        {
          close(s);
          return (0);
        }
      write(1, buff, strlen(buff));
      clean_buff(buff);
      free(buff);
      ++ligne;
    }
  return (0);
}

int	connect_clt(char *ip, char *nb_port)
{
  int	s;

  if ((s = init_socket(nb_port)) == -1)
    return (-1);
  if (creat_connection(s, ip, atoi(nb_port)) == -1)
    return (-1);
  if (go_client(s) == -1)
    return (-1);
  return (0);
}

int	main(int ac, char **av)
{
  if (check_arg(ac) == -1)
    return (-1);
  if (connect_clt(av[1], av[2]) == -1)
    return (-1);
  return (0);
}
