#include <stdio.h>
#include <unistd.h>

typedef unsigned char argsin_t;

const unsigned char ARG_PASSED 			= ((unsigned char) 1) << 0;
const unsigned char ARG_NXT_LINE 		= ((unsigned char) 1) << 1;
const unsigned char ARG_NUMBER_LINE 	= ((unsigned char) 1) << 2;
const unsigned char ARG_NO_SPACE 		= ((unsigned char) 1) << 3;
const unsigned char ARG_HELP 			= ((unsigned char) 1) << 4;
const unsigned char ARG_SHOW_ENV	 	= ((unsigned char) 1) << 5;

#define ARG_MAX_N_READ 5

#define ARG_IS_SET(args, flag) (((args) & (flag)) == (flag))

#define ARG_UNMASK(args, flag) args = args & (~flag)


#define COMMENT(cmt) ;

void echo_words(const char **, argsin_t);

void echo_string_list(const char **, argsin_t);

unsigned char parse_args(const char *);

void echo_help();

int main(const int 		argc,
		 const char ** 	argv,
		 const char ** 	argp)
{
	argsin_t args = parse_args(*(argv + 1));
	unsigned char arg_passed = ARG_IS_SET(args, ARG_PASSED);

	if(argc == 2 && ARG_IS_SET(args, ARG_HELP))
	{
		echo_help();
		return 0;
	}

	if(argc == 1 || (argc <= 2 && arg_passed))
	{
		echo_string_list(argp, args);
		return 0;
	}

	echo_words(argv + 1 + arg_passed, args);

	return 0;
}

void echo_words(const char ** words, argsin_t args)
{
	char ** _chlist = (char **) words;

	while(*_chlist)
	{
		while(**_chlist)
		{
			/**
			checks for LF
			*/
			if(**_chlist 		== '\\'  &&
			   *(*_chlist+1) 	== 'n' &&
			   ARG_IS_SET(args, ARG_NXT_LINE))
			{
				putc('\n', stdout);

				/**
				jumps \n
				*/
				*(_chlist) += 2;
			}
			else
			{
				/**
				no esc chars
				*/
				putc(*((*_chlist)++), stdout);
			}
		}

		if(!ARG_IS_SET(args, ARG_NO_SPACE))
		{
			putc(' ', stdout);
		}

		_chlist = _chlist + 1;
	}

	putc('\n', stdout);
}

void echo_string_list(const char ** chlist, argsin_t args)
{
	char ** _chlist = (char **) chlist;
	unsigned int cc = 0;

	while(*_chlist)
	{
		if(ARG_IS_SET(args, ARG_NUMBER_LINE) )
		{
			printf("%u.", cc);
			cc++;
		}

		while(**_chlist)
		{
			putc(*((*_chlist)++), stdout);
		}

		_chlist = _chlist + 1;

		if(ARG_IS_SET(args, ARG_NXT_LINE))
		{
			putc('\n', stdout);
		}
	}
}

unsigned char parse_args(const char * argv)
{
	if(argv == NULL || *argv != '-')
	{
		return 0;
	}

	char * params = (char *) argv;
	char args = 0;

	args |= ARG_PASSED;

	while(*(params++) &&
		 (((unsigned long) params - (unsigned long) argv) <= ARG_MAX_N_READ))
	{
		switch(*params)
		{
			case 'n': args |= ARG_NXT_LINE;
			break;

			case 'l': args |= ARG_NUMBER_LINE;
					  args |= ARG_NXT_LINE;
			break;

			case 'N': args |= ARG_NO_SPACE;
			break;

			case 'E': args |= ARG_SHOW_ENV;
			break;

			case 'h': args |= ARG_HELP;
			break;
		}
	}

	return args;
}

void echo_help()
{
	printf("Franklin - 2021\n\n");
	printf("-l enumerar\n");
	printf("-n pular linha\n");
	printf("-E mostrar variaveis de ambiente\n");
	printf("-h exibir ajuda\n");
}