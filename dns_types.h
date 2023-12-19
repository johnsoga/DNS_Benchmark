#ifndef DNS_TYPES_H
#define DNS_TYPES_H

typedef struct dnsheader {

    // HEADER SIZE IS 12 BYTES
    unsigned short id;          // identification number
    unsigned short flags;       // flags (qr, opcode, aa, tc, etc.)
    unsigned short qd_count;    // number of entries in question section
    unsigned short an_count;    // number of resource records in answer
    unsigned short ns_count;    // number of name server resource records in authority records section
    unsigned short ar_count;    // number of resource records in additional records section
} DNS_HEADER;

typedef struct dnsserver {
    char *DNSProviderName;
    char *DNSProviderIP;
} dnshost;

#endif
