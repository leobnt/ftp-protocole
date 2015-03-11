/*
** server.c for  in /home/benoit_l/rendu/PSU_2014_myftp/src_server
** 
** Made by Leo BENOIT
** Login   <benoit_l@epitech.net>
** 
** Started on  Wed Mar 11 10:51:04 2015 Leo BENOIT
** Last update Wed Mar 11 13:53:44 2015 Leo BENOIT
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

int	my_strlen(char *str)
{
  int	i;

  i = 0;
  while (str[i])
    ++i;
  return (i);
}

void	my_put_error(char *str)
{
  write(2, str, my_strlen(str));
}

void	my_putstr(char *str)
{
  write(1, str, my_strlen(str));
}

int	check_arg(int ac)
{
  if (ac != 2)
    {
      my_putstr("Usage: ./server [PORT]\n");
      return (-1);
    }
  return (0);
}

void	clean_buff(char *buff)
{
  int	i;

  i = 0;
  while (i < 4096)
    {
      buff[i] = '\0';
      ++i;
    }
}

int	init_socket(char *nb_port)
{
  struct protoent	*pe;
  int			port;
  int			s;

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

struct sockaddr_in	init_sin_struct(int port)
{
  struct sockaddr_in	sin;
  
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = INADDR_ANY;  
  return (sin);
}

int	init_bind(int s, struct sockaddr_in sin)
{
  int	error;
  
  error = bind(s, (const struct sockaddr *)&sin, sizeof(sin));
  if (error == -1)
    {
      my_put_error("Error: Init bind\n");
      close(s);
      return (-1);
    }
  return (0);
}

int	init_listen(int s)
{
  int	error;

  error = listen(s, 42);
  if (error == -1)
    {
      my_put_error("Error: Init listen\n");
      close(s);
      return (-1);
    }
  return (0);
}

int	creat_connection(int s, int port)
{
  struct sockaddr_in	sin;

  sin = init_sin_struct(port);
  if (init_bind(s, sin) == -1)
    return (-1);
  if (init_listen(s) == -1)
    return (-1);
  return (0);
}

void	affich_ip(struct sockaddr_in sin_client)
{
  char *str;

  str = inet_ntoa(sin_client.sin_addr);
  my_putstr("New connection -> ");
  my_putstr(str);
  my_putstr("\n");
}

int	accept_connect(int s)
{
  struct sockaddr_in	sin_client;
  socklen_t		client_len;
  int			cs;

  cs = accept(s, (struct sockaddr *)&sin_client, &client_len);
  if (cs == -1)
    {
      my_put_error("Error: Init accept\n");
      close(s);
      return (-1);
    }
  affich_ip(sin_client);
  return (cs);
}

int	go_server(int s)
{
  char *buff;
  int	cs;

  while (1)
    {
      if ((cs = accept_connect(s)) == -1)
	return (-1);
      if (fork() == 0)
      	{
      	  dup2(cs, 1);
      	  dup2(cs, 2);
	  while (42)
	    {
	      buff = malloc(sizeof(char) * 4096);
	      if (read(cs, buff, 4095) == -1)
		{
		  close(s);
		  return (-1);
		}
	      if (strcmp(buff, "exit\n") == 0)
		{
		  close(s);
		  return (0);
		}
	      system(buff);
	      clean_buff(buff);
	      free(buff);
	    }
	}
      //    else
	/* wait(NULL); */
	}
  close(s);
  return (0);
}

int	connect_srv(char *nb_port)
{
  int	s;

  if ((s = init_socket(nb_port)) == -1)
    return (-1);
  if (creat_connection(s, atoi(nb_port)) == -1)
    return (-1);
  if (go_server(s) == -1)
    return (-1);
  return (0);
}

int	main(int ac, char **av)
{
  if (check_arg(ac) == -1)
    return (-1);
  if (connect_srv(av[1]) == -1)
    return (-1);
  return (0);
}
