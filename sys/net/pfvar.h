/*	$OpenBSD: pfvar.h,v 1.10 2001/06/25 17:17:03 dhartmei Exp $ */

/*
 * Copyright (c) 2001, Daniel Hartmeier
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *    - Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer. 
 *    - Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _NET_PFVAR_H_
#define _NET_PFVAR_H_

#include <sys/types.h>

enum	{ PF_IN=0, PF_OUT=1 };
enum	{ PF_PASS=0, PF_DROP=1, PF_DROP_RST=2 };

struct pf_rule_addr {
	u_int32_t	addr;
	u_int32_t	mask;
	u_int16_t	port[2];
	u_int8_t	not;
	u_int8_t	port_op;
};

struct pf_rule {
	char		 ifname[IFNAMSIZ];
	struct ifnet	*ifp;
	struct pf_rule_addr src;
	struct pf_rule_addr dst;
	struct pf_rule	*next;

	u_int8_t	 action;
	u_int8_t	 direction;
	u_int8_t	 log;
	u_int8_t	 quick;

	u_int8_t	 keep_state;
	u_int8_t	 proto;
	u_int8_t	 type;
	u_int8_t	 code;

	u_int8_t	 flags;
	u_int8_t	 flagset;
};

struct pf_state_host {
	u_int32_t	addr;
	u_int16_t	port;
};

struct pf_state_peer {
	u_int32_t	seqlo;
	u_int32_t	seqhi;
	u_int8_t	state;
};

struct pf_state {
	struct pf_state	*next;
	struct pf_state_host lan;
	struct pf_state_host gwy;
	struct pf_state_host ext;
	struct pf_state_peer src;
	struct pf_state_peer dst;
	u_int32_t	 creation;
	u_int32_t	 expire;
	u_int32_t	 packets;
	u_int32_t	 bytes;
	u_int8_t	 proto;
	u_int8_t	 direction;
};

struct pf_nat {
	char		 ifname[IFNAMSIZ];
	struct ifnet	*ifp;
	struct pf_nat	*next;
	u_int32_t	 saddr;
	u_int32_t	 smask;
	u_int32_t	 daddr;
	u_int8_t	 proto;
	u_int8_t	 not;
};

struct pf_rdr {
	char		 ifname[IFNAMSIZ];
	struct ifnet	*ifp;
	struct pf_rdr	*next;
	u_int32_t	 daddr;
	u_int32_t	 dmask;
	u_int32_t	 raddr;
	u_int16_t	 dport;
	u_int16_t	 rport;
	u_int8_t	 proto;
	u_int8_t	 not;
};

struct pf_status {
	u_int32_t	running;
	u_int32_t	bytes[2];
	u_int32_t	packets[2][2];
	u_int32_t	states;
	u_int32_t	state_inserts;
	u_int32_t	state_removals;
	u_int32_t	state_searches;
	u_int32_t	since;
};

/*
 * ioctl parameter structures
 */

struct pfioc_rule {
	u_int32_t	 ticket;
	u_int32_t	 nr;
	struct pf_rule	 rule;
};

struct pfioc_nat {
	u_int32_t	 ticket;
	u_int32_t	 nr;
	struct pf_nat	 nat;
};

struct pfioc_rdr {
	u_int32_t	 ticket;
	u_int32_t	 nr;
	struct pf_rdr	 rdr;
};

struct pfioc_state {
	u_int32_t	 nr;
	struct pf_state	 state;
};

struct pfioc_if {
	char		 ifname[IFNAMSIZ];
};

/*
 * ioctl operations
 */

#define DIOCSTART	_IO  ('D',  1)
#define DIOCSTOP	_IO  ('D',  2)
#define DIOCBEGINRULES	_IOWR('D',  3, u_int32_t)
#define DIOCADDRULE	_IOWR('D',  4, struct pfioc_rule) 
#define DIOCCOMMITRULES	_IOWR('D',  5, u_int32_t)
#define DIOCGETRULES	_IOWR('D',  6, struct pfioc_rule)
#define DIOCGETRULE	_IOWR('D',  7, struct pfioc_rule) 
#define DIOCBEGINNATS	_IOWR('D',  8, u_int32_t)
#define DIOCADDNAT	_IOWR('D',  9, struct pfioc_nat) 
#define DIOCCOMMITNATS	_IOWR('D', 10, u_int32_t)
#define DIOCGETNATS	_IOWR('D', 11, struct pfioc_nat)
#define DIOCGETNAT	_IOWR('D', 12, struct pfioc_nat) 
#define DIOCBEGINRDRS	_IOWR('D', 13, u_int32_t)
#define DIOCADDRDR	_IOWR('D', 14, struct pfioc_rdr) 
#define DIOCCOMMITRDRS	_IOWR('D', 15, u_int32_t)
#define DIOCGETRDRS	_IOWR('D', 16, struct pfioc_rdr)
#define DIOCGETRDR	_IOWR('D', 17, struct pfioc_rdr) 
#define DIOCCLRSTATES	_IO  ('D', 18)
#define DIOCGETSTATE	_IOWR('D', 19, struct pfioc_state)
#define DIOCSETSTATUSIF _IOWR('D', 20, struct pfioc_if)
#define DIOCGETSTATUS	_IOWR('D', 21, struct pf_status)


#ifdef _KERNEL

int	pf_test(int, struct ifnet *, struct mbuf **);

#endif /* _KERNEL */

#endif /* _NET_PFVAR_H_ */
