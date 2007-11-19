// $Id$

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <pspkerneltypes.h>
#include <pspnet_resolver.h>
#include <pspnet_inet.h>

#define MAX_NAME 512

#include "pspsocket.h"

int gethostname(char *name, size_t len)
{
    FILE *fp;
    size_t a;

    if (len < 0)
    {
       errno = EINVAL;
       return -1;
    }

    fp = fopen("ms0:/python/hostname", "r");

    if (!fp)
    {
       strncpy(name, "python-psp.net", len);
       return 0;
    }

    a = fread(name, 1, len - 1, fp);
    name[a] = 0;
    fclose(fp);

    return 0;
}

int sethostname(const char *name, size_t len)
{
    FILE *fp;

    if (len < 0)
    {
       errno = EINVAL;
       return -1;
    }

    fp = fopen("ms0:/python/hostname", "w");

    if (!fp)
    {
       errno = EPERM;
       return -1;
    }

    fwrite(name, 1, len, fp);
    fclose(fp);

    return 0;
}

struct servent* getservbyname(const char *name, const char *proto)
{
    /* TODO */

    return NULL;
}

struct servent* getservbyport(int port, const char *proto)
{
    /* TODO */

    return NULL;
}

struct protoent* getprotobyname(const char *name)
{
    return NULL;
}

struct hostent *psp_gethostbyname(const char *name)
{
	static struct hostent ent;
	char buf[1024];
	static char sname[MAX_NAME] = "";
	static struct in_addr saddr = { 0 };
	static char *addrlist[2] = { (char *) &saddr, NULL };
	int rid;

	if(sceNetInetInetAton(name, &saddr) == 0)
	{
		int err;

		if(sceNetResolverCreate(&rid, buf, sizeof(buf)) < 0)
		{
			h_errno = NO_RECOVERY;
			return NULL;
		}

		err = sceNetResolverStartNtoA(rid, name, &saddr, 2, 3);

                sceNetResolverStop(rid);
		sceNetResolverDelete(rid);

		if(err < 0)
		{
			h_errno = HOST_NOT_FOUND;
			return NULL;
		}

	}

	snprintf(sname, MAX_NAME, "%s", name);
	ent.h_name = sname;
	ent.h_aliases = 0;
	ent.h_addrtype = AF_INET;
	ent.h_length = sizeof(struct in_addr);
	ent.h_addr_list = addrlist;
	ent.h_addr = (char *) &saddr;

	return &ent;
}

struct hostent *psp_gethostbyaddr(const void *addr, int len, int type)
{
        static struct hostent ent;
        static char* aliases[1] = { NULL };
        char buf[1024];
        static char sname[MAX_NAME] = "";
        static struct in_addr saddr = { 0 };
        static char *addrlist[2] = { (char *) &saddr, NULL };
        int rid;
        int err;

        if((len != sizeof(struct in_addr)) || (type != AF_INET) || (addr == NULL))
        {
                h_errno = HOST_NOT_FOUND;
                return NULL;
        }

        memcpy(&saddr, addr, len);

        if(sceNetResolverCreate(&rid, buf, sizeof(buf)) < 0)
        {
                h_errno = NO_RECOVERY;
                return NULL;
        }

        err = sceNetResolverStartAtoN(rid, &saddr, sname, sizeof(sname), 2, 3);

        sceNetResolverStop(rid);
        sceNetResolverDelete(rid);

        if(err < 0)
        {
                h_errno = HOST_NOT_FOUND;
                return NULL;
        }

        ent.h_name = sname;
        ent.h_aliases = aliases;
        ent.h_addrtype = AF_INET;
        ent.h_length = sizeof(struct in_addr);
        ent.h_addr_list = addrlist;
        ent.h_addr = (char *) &saddr;

        return &ent;
}

#ifdef GNUC
static const char* _rcsid_pspsocket __attribute__((unused)) = "$Id$";
#endif
