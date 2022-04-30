#ifndef __MCP23S018_H__
#define __MCP23S018_H__

#define IODIR		0x00
#define IPOL		0x01
#define GPINTEN		0x02
#define DEFVAL		0x03
#define INTCON		0x04
#define IOCON		0x05
#define GPPU		0x06
#define INTF		0x07
#define INTCAP		0x08
#define GPIO		0x09
#define OLAT		0x0A

class MCP23S018
{

private:
    /* data */
public:
    MCP23S018(/* args */);
    ~MCP23S018();
    
};

#endif /* __MCP23S018_H__ */
