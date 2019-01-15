#include <stdio.h>
#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

void	print_output(int nsyms, int symoff, int stroff, char *ptr)
{
	int		i;
	char	*stringtable;
	struct nlist_64	*array;

	array = (void *)ptr + symoff;
	stringtable = (void *) ptr + stroff;
	for (i = 0; i < nsyms; ++i)
	{
		printf("%s\n", stringtable + array[i].n_un.n_strx);
	}
}

void	handle_64(char *ptr)
{
	int						ncmds;
	int						i;
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;

	header = (struct mach_header_64 *)ptr;
	ncmds = header->ncmds;
	lc = (void *)ptr + sizeof(*header);
	for (i = 0; i < ncmds; ++i)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *) lc;
			print_output(sym->nsyms, sym->symoff, sym->stroff, ptr);
			break;
		}
		lc = (void *)lc + lc->cmdsize;
	}
}

void	nm(char *ptr)
{
	int	magic;

	magic = *(int *) ptr;
	if (magic == MH_MAGIC_64)
		handle_64(ptr);
}

int		main(int ac, char **av)
{
	int			fd;
	char		*ptr;
	struct stat	buff;

	if (ac != 2)
	{
		fprintf(stderr, "Please gimmi an arg\n");
		return (EXIT_FAILURE);
	}
	if ((fd = open(av[1], O_RDONLY)) < 0)
	{
		perror("open");
		return (EXIT_FAILURE);
	}
	if (fstat(fd, &buff) < 0)
	{
		perror("fstat");
		return (EXIT_FAILURE);
	}
	if ((ptr = mmap(0, buff.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		perror("mmap");
		return (EXIT_FAILURE);
	}
	nm(ptr);
	if (munmap(ptr, buff.st_size) < 0)
	{
		perror("munmap");
		return (EXIT_FAILURE);
	}
}
