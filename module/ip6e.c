#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/net.h>
#include <net/sock.h>
#include <net/inet_sock.h>

struct ip6e_sock {
   struct inet_sock isk;
   //my implementation data members here now??? or should we be using the ipv6 impl socket?
} ip6e_sock;

static void ip6e_close(struct sock* sk, long timeout) {
}

static int ip6e_connect(struct sock* sk, struct sockaddr* uaddr, int addr_len) {
   return 0;
}

static int ip6e_disconnect(struct sock* sk, int flags) {
   return 0;
}

/*static struct sock* ip6e_accept(struct sock* sk, int flags, int* err) {
   return NULL;
   }*/

static int ip6e_ioctl(struct sock* sk, int cmd, unsigned long arg) {
   return 0;
}

static int ip6e_init(struct sock* sk) {
   return 0;
}

/*static void ip6e_shutdown(struct sock* sk, int how) {
  }*/

static int ip6e_getsockopt(struct sock* sk, int level, int optname, char* optval, int* option) {
   return 0;
}

static int ip6e_setsockopt(struct sock* sk, int level, int optname, char* optval, unsigned int optlen) {
   return 0;
}

static int ip6e_sendmsg(struct kiocb* iocb, struct sock* sk, struct msghdr* msg, size_t len) {
   return 0;
}

static int ip6e_recvmsg(struct kiocb* iocb, struct sock* sk, struct msghdr* msg,
                           size_t len, int noblock, int flags, int* addr_len) {
   return 0;
}

static void ip6e_destroy(struct sock* sk) {
}

static int ip6e_bind(struct sock* sk, struct sockaddr* uaddr, int addr_len) {
   return 0;
}

static int ip6e_backlog_rcv(struct sock* sk, struct sk_buff* skb) {
   return 0;
}

static void ip6e_hash(struct sock* sk) {
}

static void ip6e_unhash(struct sock* sk) {
}

static void ip6e_release_cb(struct sock* sk) {
}

static int compat_ip6e_setsockopt(struct sock *sk, int level, int optname, char __user *optval, unsigned int optlen) {
   return 0;
}

static int compat_ip6e_getsockopt(struct sock *sk, int level, int optname, char __user *optval, int __user *option) {
   return 0;
}

static int compat_ip6e_ioctl(struct sock *sk, unsigned int cmd, unsigned long arg) {
   return 0;
}

static int __init ip6e_modinit(void) {
   int res;
   //struct net_proto_family npf;
   struct proto proto_raw = {
      .name = "IP6E_RAW",
      .owner = THIS_MODULE,

      .close = ip6e_close,
      .destroy = ip6e_destroy,
      .connect = ip6e_connect,
      .disconnect = ip6e_disconnect,

      .ioctl = ip6e_ioctl,
      .init = ip6e_init,
      .setsockopt = ip6e_setsockopt,
      .getsockopt = ip6e_getsockopt,

      .sendmsg = ip6e_sendmsg,
      .recvmsg = ip6e_recvmsg,
      .bind = ip6e_bind,
      .backlog_rcv = ip6e_backlog_rcv,
      
      .release_cb = ip6e_release_cb,
      .hash = ip6e_hash,
      .unhash = ip6e_unhash,
      .obj_size = sizeof(struct ip6e_sock),

      //.h.raw_hash = ,
#ifdef CONFIG_COMPAT
      .compat_setsockopt = compat_ip6e_setsockopt,
      .compat_getsockopt = compat_ip6e_getsockopt,
      .compat_ioctl      = compat_ip6e_ioctl
#endif
   };

   res = proto_register(&proto_raw, 1);
   if (res) {
      printk(KERN_INFO "Error registering proto (%d).\n", res);
   } else {
      printk(KERN_INFO "Registered protocol successfully\n");
   }

   proto_unregister(&proto_raw);
   
   printk(KERN_INFO "Done.\n");

   return 0; //non-0 return means the module failed
}

static void __exit ip6e_modexit(void) {
   printk(KERN_INFO "Goodbye.\n");
}

module_init(ip6e_modinit);
module_exit(ip6e_modexit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Jacob Hladky");
MODULE_DESCRIPTION("Test driver for ip6e project.");

