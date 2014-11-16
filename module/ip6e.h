
#define AF_IP6E 28 /*So basically this number was unused when I looked at the table in socket.h... not sure what else I could do here...*/

static struct ip6e_sock {
   struct inet_sock isk;
   //my implementation data members here now??? or should we be using the ipv6 impl socket?
} ip6e_sock;

static void ip6e_close(struct sock* sk, long timeout);
static int ip6e_connect(struct sock* sk, struct sockaddr* uaddr, int addr_len);
static int ip6e_disconnect(struct sock* sk, int flags);
/*static struct sock* ip6e_accept(struct sock* sk, int flags, int* err);*/
static int ip6e_ioctl(struct sock* sk, int cmd, unsigned long arg);
static int ip6e_init(struct sock* sk);
/*static void ip6e_shutdown(struct sock* sk, int how);*/
static int ip6e_getsockopt(struct sock* sk, int level, int optname, char* optval, int* option);
static int ip6e_setsockopt(struct sock* sk, int level, int optname, char* optval, unsigned int optlen);
static int ip6e_sendmsg(struct kiocb* iocb, struct sock* sk, struct msghdr* msg, size_t len);
static int ip6e_recvmsg(struct kiocb* iocb, struct sock* sk, struct msghdr* msg,
                        size_t len, int noblock, int flags, int* addr_len);
static void ip6e_destroy(struct sock* sk);
static int ip6e_bind(struct sock* sk, struct sockaddr* uaddr, int addr_len);
static int ip6e_backlog_rcv(struct sock* sk, struct sk_buff* skb);
static void ip6e_hash(struct sock* sk);
static void ip6e_unhash(struct sock* sk);
static void ip6e_release_cb(struct sock* sk);
static int compat_ip6e_setsockopt(struct sock *sk, int level, int optname, char __user *optval, unsigned int optlen);
static int compat_ip6e_getsockopt(struct sock *sk, int level, int optname, char __user *optval, int __user *option);
static int compat_ip6e_ioctl(struct sock *sk, unsigned int cmd, unsigned long arg);
static int ip6e_create(struct net *net, struct socket *sock, int protocol, int kern);
