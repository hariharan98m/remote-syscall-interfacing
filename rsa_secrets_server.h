const double p= 3;
const double q =7;
const int A=4;

#define n p*q

#define phi (p-1)*(q-1)


enum message_type {a=3, SUCCESS=20, FAILURE=30, SIP=1, ASYMMETRIC= 2, SYMMETRIC=3, RSA=4, DE=5, HANDSHAKE=6, REQ_CERTIFICATE=7, RES_CERTIFICATE=8, KEYS_SAVED=9, ENC_MSG=10, DECR_MSG=11, CONN_AUTH_SUCCESS=12, SESSION_TOKEN_SENT=13, USER_INFO=14, OK=15, MSG_TYPE=16, DE_XCHG=17, G_SECRET=18, USER=1, ADMIN=0, RSAKEY=19};


void error(const char* err){
	printf("%s", err);
	exit(0);
}


struct test{
	int a,b;
	char str[30];
};

struct message{
	int type;
	char payload[30];
	char sessionToken[30];
	int privLevel;
};


typedef struct user{
	char username[20];
	char password[20];
	long int sessionToken;
	long int privLevel;
}user;


typedef struct encDecMsg{
	int cmd[20];
}encDecMsg;


typedef struct handshake{
	long int num;
	int protocol_v;
	int cipher_suite;
	int algorithm;
}handshake;


typedef struct enc_msg{

	double enc;
	double e;

}enc_msg;

typedef struct DHkeys{
	long int P, G;
	long int secret;
}DHkeys;



typedef struct keys{
	double N;
	double PHI;
}keys;


int func(){
	return 10;
}

// Returns gcd of a and b
int gcd(int a, int h)
{
    int temp;
    while (1)
    {
        temp = a%h;
        if (temp == 0)
          return h;
        a = h;
        h = temp;
    }
}

