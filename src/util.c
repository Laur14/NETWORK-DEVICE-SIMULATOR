#include "util.h"

char* protocol_costum(int a){
   switch (a)
   {
    case 2 :
    return "udp_costume";
            break;
   
    default:
    return "NULL";
            break;
   }
}



void packet_deserialize(const uint8_t* buf,ssize_t lenght, packet_t* pk){
    if((lenght - dPayload) >= 0 && (size_t)(lenght - dPayload) <= sizeof(pk->payload))
    {
        uint16_t l = lenght - dPayload;
        memcpy(pk->payload,buf+dPayload,l);
    }
    else
    {
        perror("invalid lenght");
        return;
    }

    uint32_t tmp32;
    uint16_t tmp16;
   
    memcpy(&tmp32,buf+srcip,4);
    pk->src_ip = ntohl(tmp32);

    memcpy(&tmp32,buf+dstip,4);
    pk->dst_ip = ntohl(tmp32);

    memcpy(&tmp16,buf+srcport,2);
    pk->src_port = ntohs(tmp16);

    memcpy(&tmp16,buf+dstport,2);
    pk->dst_port = ntohs(tmp16);

    pk->protocol = buf[dprotocol];

    memcpy(&tmp16,buf+dlength,2);
    pk->length = ntohs(tmp16);
    
}


size_t packet_serialized(packet_t pk, uint8_t* buff){
    uint16_t temp16;
    uint32_t temp32;
    temp32 = htonl(pk.src_ip);
    memcpy(buff + srcip,&temp32,4);

    temp32 = htonl(pk.dst_ip);
    memcpy(buff+dstip,&temp32,4);

    temp16 = htons(pk.src_port);
    memcpy(buff+srcport,&temp16,2);

    temp16 = htons(pk.dst_port);
    memcpy(buff+dstport,&temp16,2);

    buff[dprotocol] = pk.protocol;

    temp16 = htons(pk.length);
    memcpy(buff+dlength,&temp16,2);


    memcpy(buff+dPayload,&pk.payload,pk.length);
    return pk.length+dPayload;
}