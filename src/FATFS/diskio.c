/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h" /* FatFs lower layer API */
//#include "usbdisk.h"    /* Example: USB drive control */
//#include "atadrive.h"    /* Example: ATA drive control */
#include "w25q16.h" /* Example: MMC/SDC contorl */
#include "ffconf.h"
//#include "commu.h"

/* Definitions of physical drive number for each media */
//#define ATA        0
#define MMC 0
//#define USB        2

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/
//void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //??flash
//void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//??flash
//void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);

#define MY_FLASH_CMD_READ 0
#define MY_FLASH_CMD_WRITE 1
void MMC_disk_read(u8 *pBuffer, u32 sector_addr, u16 NumSectorToRead)
{
    SPI_Flash_Read(pBuffer, sector_addr * _MAX_SS, NumSectorToRead * _MAX_SS);
}
void MMC_disk_write(u8 *pBuffer, u32 sector_addr, u16 NumSectorToWrite)
{
    SPI_Flash_Write(pBuffer, sector_addr * _MAX_SS, NumSectorToWrite * _MAX_SS);
}

DSTATUS disk_initialize(
    BYTE pdrv /* Physical drive nmuber (0..) */
)
{
    //    DSTATUS stat;
    //    int result;

    return 0;
}

/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
    BYTE pdrv /* Physical drive nmuber (0..) */
)
{
    //    DSTATUS stat;
    //    int result;
    // translate the reslut code here

    return 0;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
    BYTE pdrv,    /* Physical drive nmuber (0..) */
    BYTE *buff,   /* Data buffer to store read data */
    DWORD sector, /* Sector address (LBA) */
    UINT count    /* Number of sectors to read (1..128) */
)
{
    MMC_disk_read(buff, sector, count);
    // translate the reslut code here
    return RES_OK;
}

    /*-----------------------------------------------------------------------*/
    /* Write Sector(s)                                                       */
    /*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write(
    BYTE pdrv,    /* Physical drive nmuber (0..) */
    BYTE *buff,   /* Data to be written */
    DWORD sector, /* Sector address (LBA) */
    UINT count    /* Number of sectors to write (1..128) */
)
{
    MMC_disk_write(buff, sector, count);

    // translate the reslut code here

    return RES_OK;
}
#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
DRESULT MMC_disk_ioctl(BYTE cmd, void *buff)
{
    return RES_OK;
}
#if _USE_IOCTL
DRESULT disk_ioctl(
    BYTE pdrv, /* Physical drive nmuber (0..) */
    BYTE cmd,  /* Control code */
    void *buff /* Buffer to send/receive control data */
)
{

    // post-process here

    return MMC_disk_ioctl(cmd, buff);
}
#endif
DWORD get_fattime()
{
    return 0;
}
