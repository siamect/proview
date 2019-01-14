"use strict";

/** Crypt start **/
/****************************************************************************
 * Proview
 *
 * Java-based implementation of the unix crypt command
 *
 * Based upon C source code written by Eric Young, eay@psych.uq.oz.au
 *
 * Author John Dumas, original name JCrypt
 *
 * Used and distributed with Proview by permission of the author.
 *
 ****************************************************************************/

/**
    Java-based implementation of the unix crypt command
    Based upon C source code written by Eric Young, eay@psych.uq.oz.au
    and the java version JCrypt by John Dumas.
*/
function JopCrypt() {
  this.ITERATIONS = 16;

  this.con_salt =
    [
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
      0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
      0x0A, 0x0B, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,
      0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12,
      0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A,
      0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22,
      0x23, 0x24, 0x25, 0x20, 0x21, 0x22, 0x23, 0x24,
      0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C,
      0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34,
      0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C,
      0x3D, 0x3E, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00
    ];

  this.shifts2 =
    [
      false, false, true, true, true, true, true, true,
      false, true, true, true, true, true, true, false
    ];

  this.skb =
    [
      [
        /* for C bits (numbered as per FIPS 46) 1 2 3 4 5 6 */
        0x00000000, 0x00000010, 0x20000000, 0x20000010,
        0x00010000, 0x00010010, 0x20010000, 0x20010010,
        0x00000800, 0x00000810, 0x20000800, 0x20000810,
        0x00010800, 0x00010810, 0x20010800, 0x20010810,
        0x00000020, 0x00000030, 0x20000020, 0x20000030,
        0x00010020, 0x00010030, 0x20010020, 0x20010030,
        0x00000820, 0x00000830, 0x20000820, 0x20000830,
        0x00010820, 0x00010830, 0x20010820, 0x20010830,
        0x00080000, 0x00080010, 0x20080000, 0x20080010,
        0x00090000, 0x00090010, 0x20090000, 0x20090010,
        0x00080800, 0x00080810, 0x20080800, 0x20080810,
        0x00090800, 0x00090810, 0x20090800, 0x20090810,
        0x00080020, 0x00080030, 0x20080020, 0x20080030,
        0x00090020, 0x00090030, 0x20090020, 0x20090030,
        0x00080820, 0x00080830, 0x20080820, 0x20080830,
        0x00090820, 0x00090830, 0x20090820, 0x20090830
      ],
      [
        /* for C bits (numbered as per FIPS 46) 7 8 10 11 12 13 */
        0x00000000, 0x02000000, 0x00002000, 0x02002000,
        0x00200000, 0x02200000, 0x00202000, 0x02202000,
        0x00000004, 0x02000004, 0x00002004, 0x02002004,
        0x00200004, 0x02200004, 0x00202004, 0x02202004,
        0x00000400, 0x02000400, 0x00002400, 0x02002400,
        0x00200400, 0x02200400, 0x00202400, 0x02202400,
        0x00000404, 0x02000404, 0x00002404, 0x02002404,
        0x00200404, 0x02200404, 0x00202404, 0x02202404,
        0x10000000, 0x12000000, 0x10002000, 0x12002000,
        0x10200000, 0x12200000, 0x10202000, 0x12202000,
        0x10000004, 0x12000004, 0x10002004, 0x12002004,
        0x10200004, 0x12200004, 0x10202004, 0x12202004,
        0x10000400, 0x12000400, 0x10002400, 0x12002400,
        0x10200400, 0x12200400, 0x10202400, 0x12202400,
        0x10000404, 0x12000404, 0x10002404, 0x12002404,
        0x10200404, 0x12200404, 0x10202404, 0x12202404
      ],
      [
        /* for C bits (numbered as per FIPS 46) 14 15 16 17 19 20 */
        0x00000000, 0x00000001, 0x00040000, 0x00040001,
        0x01000000, 0x01000001, 0x01040000, 0x01040001,
        0x00000002, 0x00000003, 0x00040002, 0x00040003,
        0x01000002, 0x01000003, 0x01040002, 0x01040003,
        0x00000200, 0x00000201, 0x00040200, 0x00040201,
        0x01000200, 0x01000201, 0x01040200, 0x01040201,
        0x00000202, 0x00000203, 0x00040202, 0x00040203,
        0x01000202, 0x01000203, 0x01040202, 0x01040203,
        0x08000000, 0x08000001, 0x08040000, 0x08040001,
        0x09000000, 0x09000001, 0x09040000, 0x09040001,
        0x08000002, 0x08000003, 0x08040002, 0x08040003,
        0x09000002, 0x09000003, 0x09040002, 0x09040003,
        0x08000200, 0x08000201, 0x08040200, 0x08040201,
        0x09000200, 0x09000201, 0x09040200, 0x09040201,
        0x08000202, 0x08000203, 0x08040202, 0x08040203,
        0x09000202, 0x09000203, 0x09040202, 0x09040203
      ],
      [
        /* for C bits (numbered as per FIPS 46) 21 23 24 26 27 28 */
        0x00000000, 0x00100000, 0x00000100, 0x00100100,
        0x00000008, 0x00100008, 0x00000108, 0x00100108,
        0x00001000, 0x00101000, 0x00001100, 0x00101100,
        0x00001008, 0x00101008, 0x00001108, 0x00101108,
        0x04000000, 0x04100000, 0x04000100, 0x04100100,
        0x04000008, 0x04100008, 0x04000108, 0x04100108,
        0x04001000, 0x04101000, 0x04001100, 0x04101100,
        0x04001008, 0x04101008, 0x04001108, 0x04101108,
        0x00020000, 0x00120000, 0x00020100, 0x00120100,
        0x00020008, 0x00120008, 0x00020108, 0x00120108,
        0x00021000, 0x00121000, 0x00021100, 0x00121100,
        0x00021008, 0x00121008, 0x00021108, 0x00121108,
        0x04020000, 0x04120000, 0x04020100, 0x04120100,
        0x04020008, 0x04120008, 0x04020108, 0x04120108,
        0x04021000, 0x04121000, 0x04021100, 0x04121100,
        0x04021008, 0x04121008, 0x04021108, 0x04121108
      ],
      [
        /* for D bits (numbered as per FIPS 46) 1 2 3 4 5 6 */
        0x00000000, 0x10000000, 0x00010000, 0x10010000,
        0x00000004, 0x10000004, 0x00010004, 0x10010004,
        0x20000000, 0x30000000, 0x20010000, 0x30010000,
        0x20000004, 0x30000004, 0x20010004, 0x30010004,
        0x00100000, 0x10100000, 0x00110000, 0x10110000,
        0x00100004, 0x10100004, 0x00110004, 0x10110004,
        0x20100000, 0x30100000, 0x20110000, 0x30110000,
        0x20100004, 0x30100004, 0x20110004, 0x30110004,
        0x00001000, 0x10001000, 0x00011000, 0x10011000,
        0x00001004, 0x10001004, 0x00011004, 0x10011004,
        0x20001000, 0x30001000, 0x20011000, 0x30011000,
        0x20001004, 0x30001004, 0x20011004, 0x30011004,
        0x00101000, 0x10101000, 0x00111000, 0x10111000,
        0x00101004, 0x10101004, 0x00111004, 0x10111004,
        0x20101000, 0x30101000, 0x20111000, 0x30111000,
        0x20101004, 0x30101004, 0x20111004, 0x30111004
      ],
      [
        /* for D bits (numbered as per FIPS 46) 8 9 11 12 13 14 */
        0x00000000, 0x08000000, 0x00000008, 0x08000008,
        0x00000400, 0x08000400, 0x00000408, 0x08000408,
        0x00020000, 0x08020000, 0x00020008, 0x08020008,
        0x00020400, 0x08020400, 0x00020408, 0x08020408,
        0x00000001, 0x08000001, 0x00000009, 0x08000009,
        0x00000401, 0x08000401, 0x00000409, 0x08000409,
        0x00020001, 0x08020001, 0x00020009, 0x08020009,
        0x00020401, 0x08020401, 0x00020409, 0x08020409,
        0x02000000, 0x0A000000, 0x02000008, 0x0A000008,
        0x02000400, 0x0A000400, 0x02000408, 0x0A000408,
        0x02020000, 0x0A020000, 0x02020008, 0x0A020008,
        0x02020400, 0x0A020400, 0x02020408, 0x0A020408,
        0x02000001, 0x0A000001, 0x02000009, 0x0A000009,
        0x02000401, 0x0A000401, 0x02000409, 0x0A000409,
        0x02020001, 0x0A020001, 0x02020009, 0x0A020009,
        0x02020401, 0x0A020401, 0x02020409, 0x0A020409
      ],
      [
        /* for D bits (numbered as per FIPS 46) 16 17 18 19 20 21 */
        0x00000000, 0x00000100, 0x00080000, 0x00080100,
        0x01000000, 0x01000100, 0x01080000, 0x01080100,
        0x00000010, 0x00000110, 0x00080010, 0x00080110,
        0x01000010, 0x01000110, 0x01080010, 0x01080110,
        0x00200000, 0x00200100, 0x00280000, 0x00280100,
        0x01200000, 0x01200100, 0x01280000, 0x01280100,
        0x00200010, 0x00200110, 0x00280010, 0x00280110,
        0x01200010, 0x01200110, 0x01280010, 0x01280110,
        0x00000200, 0x00000300, 0x00080200, 0x00080300,
        0x01000200, 0x01000300, 0x01080200, 0x01080300,
        0x00000210, 0x00000310, 0x00080210, 0x00080310,
        0x01000210, 0x01000310, 0x01080210, 0x01080310,
        0x00200200, 0x00200300, 0x00280200, 0x00280300,
        0x01200200, 0x01200300, 0x01280200, 0x01280300,
        0x00200210, 0x00200310, 0x00280210, 0x00280310,
        0x01200210, 0x01200310, 0x01280210, 0x01280310
      ],
      [
        /* for D bits (numbered as per FIPS 46) 22 23 24 25 27 28 */
        0x00000000, 0x04000000, 0x00040000, 0x04040000,
        0x00000002, 0x04000002, 0x00040002, 0x04040002,
        0x00002000, 0x04002000, 0x00042000, 0x04042000,
        0x00002002, 0x04002002, 0x00042002, 0x04042002,
        0x00000020, 0x04000020, 0x00040020, 0x04040020,
        0x00000022, 0x04000022, 0x00040022, 0x04040022,
        0x00002020, 0x04002020, 0x00042020, 0x04042020,
        0x00002022, 0x04002022, 0x00042022, 0x04042022,
        0x00000800, 0x04000800, 0x00040800, 0x04040800,
        0x00000802, 0x04000802, 0x00040802, 0x04040802,
        0x00002800, 0x04002800, 0x00042800, 0x04042800,
        0x00002802, 0x04002802, 0x00042802, 0x04042802,
        0x00000820, 0x04000820, 0x00040820, 0x04040820,
        0x00000822, 0x04000822, 0x00040822, 0x04040822,
        0x00002820, 0x04002820, 0x00042820, 0x04042820,
        0x00002822, 0x04002822, 0x00042822, 0x04042822
      ]
    ];

  this.SPtrans =
    [
      [
        /* nibble 0 */
        0x00820200, 0x00020000, 0x80800000, 0x80820200,
        0x00800000, 0x80020200, 0x80020000, 0x80800000,
        0x80020200, 0x00820200, 0x00820000, 0x80000200,
        0x80800200, 0x00800000, 0x00000000, 0x80020000,
        0x00020000, 0x80000000, 0x00800200, 0x00020200,
        0x80820200, 0x00820000, 0x80000200, 0x00800200,
        0x80000000, 0x00000200, 0x00020200, 0x80820000,
        0x00000200, 0x80800200, 0x80820000, 0x00000000,
        0x00000000, 0x80820200, 0x00800200, 0x80020000,
        0x00820200, 0x00020000, 0x80000200, 0x00800200,
        0x80820000, 0x00000200, 0x00020200, 0x80800000,
        0x80020200, 0x80000000, 0x80800000, 0x00820000,
        0x80820200, 0x00020200, 0x00820000, 0x80800200,
        0x00800000, 0x80000200, 0x80020000, 0x00000000,
        0x00020000, 0x00800000, 0x80800200, 0x00820200,
        0x80000000, 0x80820000, 0x00000200, 0x80020200
      ],
      [
        /* nibble 1 */
        0x10042004, 0x00000000, 0x00042000, 0x10040000,
        0x10000004, 0x00002004, 0x10002000, 0x00042000,
        0x00002000, 0x10040004, 0x00000004, 0x10002000,
        0x00040004, 0x10042000, 0x10040000, 0x00000004,
        0x00040000, 0x10002004, 0x10040004, 0x00002000,
        0x00042004, 0x10000000, 0x00000000, 0x00040004,
        0x10002004, 0x00042004, 0x10042000, 0x10000004,
        0x10000000, 0x00040000, 0x00002004, 0x10042004,
        0x00040004, 0x10042000, 0x10002000, 0x00042004,
        0x10042004, 0x00040004, 0x10000004, 0x00000000,
        0x10000000, 0x00002004, 0x00040000, 0x10040004,
        0x00002000, 0x10000000, 0x00042004, 0x10002004,
        0x10042000, 0x00002000, 0x00000000, 0x10000004,
        0x00000004, 0x10042004, 0x00042000, 0x10040000,
        0x10040004, 0x00040000, 0x00002004, 0x10002000,
        0x10002004, 0x00000004, 0x10040000, 0x00042000
      ],
      [
        /* nibble 2 */
        0x41000000, 0x01010040, 0x00000040, 0x41000040,
        0x40010000, 0x01000000, 0x41000040, 0x00010040,
        0x01000040, 0x00010000, 0x01010000, 0x40000000,
        0x41010040, 0x40000040, 0x40000000, 0x41010000,
        0x00000000, 0x40010000, 0x01010040, 0x00000040,
        0x40000040, 0x41010040, 0x00010000, 0x41000000,
        0x41010000, 0x01000040, 0x40010040, 0x01010000,
        0x00010040, 0x00000000, 0x01000000, 0x40010040,
        0x01010040, 0x00000040, 0x40000000, 0x00010000,
        0x40000040, 0x40010000, 0x01010000, 0x41000040,
        0x00000000, 0x01010040, 0x00010040, 0x41010000,
        0x40010000, 0x01000000, 0x41010040, 0x40000000,
        0x40010040, 0x41000000, 0x01000000, 0x41010040,
        0x00010000, 0x01000040, 0x41000040, 0x00010040,
        0x01000040, 0x00000000, 0x41010000, 0x40000040,
        0x41000000, 0x40010040, 0x00000040, 0x01010000
      ],
      [
        /* nibble 3 */
        0x00100402, 0x04000400, 0x00000002, 0x04100402,
        0x00000000, 0x04100000, 0x04000402, 0x00100002,
        0x04100400, 0x04000002, 0x04000000, 0x00000402,
        0x04000002, 0x00100402, 0x00100000, 0x04000000,
        0x04100002, 0x00100400, 0x00000400, 0x00000002,
        0x00100400, 0x04000402, 0x04100000, 0x00000400,
        0x00000402, 0x00000000, 0x00100002, 0x04100400,
        0x04000400, 0x04100002, 0x04100402, 0x00100000,
        0x04100002, 0x00000402, 0x00100000, 0x04000002,
        0x00100400, 0x04000400, 0x00000002, 0x04100000,
        0x04000402, 0x00000000, 0x00000400, 0x00100002,
        0x00000000, 0x04100002, 0x04100400, 0x00000400,
        0x04000000, 0x04100402, 0x00100402, 0x00100000,
        0x04100402, 0x00000002, 0x04000400, 0x00100402,
        0x00100002, 0x00100400, 0x04100000, 0x04000402,
        0x00000402, 0x04000000, 0x04000002, 0x04100400
      ],
      [
        /* nibble 4 */
        0x02000000, 0x00004000, 0x00000100, 0x02004108,
        0x02004008, 0x02000100, 0x00004108, 0x02004000,
        0x00004000, 0x00000008, 0x02000008, 0x00004100,
        0x02000108, 0x02004008, 0x02004100, 0x00000000,
        0x00004100, 0x02000000, 0x00004008, 0x00000108,
        0x02000100, 0x00004108, 0x00000000, 0x02000008,
        0x00000008, 0x02000108, 0x02004108, 0x00004008,
        0x02004000, 0x00000100, 0x00000108, 0x02004100,
        0x02004100, 0x02000108, 0x00004008, 0x02004000,
        0x00004000, 0x00000008, 0x02000008, 0x02000100,
        0x02000000, 0x00004100, 0x02004108, 0x00000000,
        0x00004108, 0x02000000, 0x00000100, 0x00004008,
        0x02000108, 0x00000100, 0x00000000, 0x02004108,
        0x02004008, 0x02004100, 0x00000108, 0x00004000,
        0x00004100, 0x02004008, 0x02000100, 0x00000108,
        0x00000008, 0x00004108, 0x02004000, 0x02000008
      ],
      [
        /* nibble 5 */
        0x20000010, 0x00080010, 0x00000000, 0x20080800,
        0x00080010, 0x00000800, 0x20000810, 0x00080000,
        0x00000810, 0x20080810, 0x00080800, 0x20000000,
        0x20000800, 0x20000010, 0x20080000, 0x00080810,
        0x00080000, 0x20000810, 0x20080010, 0x00000000,
        0x00000800, 0x00000010, 0x20080800, 0x20080010,
        0x20080810, 0x20080000, 0x20000000, 0x00000810,
        0x00000010, 0x00080800, 0x00080810, 0x20000800,
        0x00000810, 0x20000000, 0x20000800, 0x00080810,
        0x20080800, 0x00080010, 0x00000000, 0x20000800,
        0x20000000, 0x00000800, 0x20080010, 0x00080000,
        0x00080010, 0x20080810, 0x00080800, 0x00000010,
        0x20080810, 0x00080800, 0x00080000, 0x20000810,
        0x20000010, 0x20080000, 0x00080810, 0x00000000,
        0x00000800, 0x20000010, 0x20000810, 0x20080800,
        0x20080000, 0x00000810, 0x00000010, 0x20080010
      ],
      [
        /* nibble 6 */
        0x00001000, 0x00000080, 0x00400080, 0x00400001,
        0x00401081, 0x00001001, 0x00001080, 0x00000000,
        0x00400000, 0x00400081, 0x00000081, 0x00401000,
        0x00000001, 0x00401080, 0x00401000, 0x00000081,
        0x00400081, 0x00001000, 0x00001001, 0x00401081,
        0x00000000, 0x00400080, 0x00400001, 0x00001080,
        0x00401001, 0x00001081, 0x00401080, 0x00000001,
        0x00001081, 0x00401001, 0x00000080, 0x00400000,
        0x00001081, 0x00401000, 0x00401001, 0x00000081,
        0x00001000, 0x00000080, 0x00400000, 0x00401001,
        0x00400081, 0x00001081, 0x00001080, 0x00000000,
        0x00000080, 0x00400001, 0x00000001, 0x00400080,
        0x00000000, 0x00400081, 0x00400080, 0x00001080,
        0x00000081, 0x00001000, 0x00401081, 0x00400000,
        0x00401080, 0x00000001, 0x00001001, 0x00401081,
        0x00400001, 0x00401080, 0x00401000, 0x00001001
      ],
      [
        /* nibble 7 */
        0x08200020, 0x08208000, 0x00008020, 0x00000000,
        0x08008000, 0x00200020, 0x08200000, 0x08208020,
        0x00000020, 0x08000000, 0x00208000, 0x00008020,
        0x00208020, 0x08008020, 0x08000020, 0x08200000,
        0x00008000, 0x00208020, 0x00200020, 0x08008000,
        0x08208020, 0x08000020, 0x00000000, 0x00208000,
        0x08000000, 0x00200000, 0x08008020, 0x08200020,
        0x00200000, 0x00008000, 0x08208000, 0x00000020,
        0x00200000, 0x00008000, 0x08000020, 0x08208020,
        0x00008020, 0x08000000, 0x00000000, 0x00208000,
        0x08200020, 0x08008020, 0x08008000, 0x00200020,
        0x08208000, 0x00000020, 0x00200020, 0x08008000,
        0x08208020, 0x00200000, 0x08200000, 0x08000020,
        0x00208000, 0x00008020, 0x08008020, 0x08200000,
        0x00000020, 0x08208000, 0x00208020, 0x00000000,
        0x08000000, 0x08200020, 0x00008000, 0x00208020
      ]
    ];

  this.cov_2char =
    [
      0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
      0x36, 0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44,
      0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C,
      0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54,
      0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x61, 0x62,
      0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A,
      0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72,
      0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A
    ];

  this.byteToUnsigned = function (b) {
    var value = Math.floor(b);

    return (value >= 0 ? value : value + 256);
  };

  this.fourBytesToInt = function (b, offset) {
    var value;

    value = this.byteToUnsigned(b[offset++]);
    value |= (this.byteToUnsigned(b[offset++]) << 8);
    value |= (this.byteToUnsigned(b[offset++]) << 16);
    value |= (this.byteToUnsigned(b[offset]) << 24);

    return value;
  };

  this.intToFourBytes = function (iValue, b, offset) {
    b[offset++] = ((iValue) & 0xff);
    b[offset++] = ((iValue >>> 8) & 0xff);
    b[offset++] = ((iValue >>> 16) & 0xff);
    b[offset] = ((iValue >>> 24) & 0xff);
  };

  this.PERM_OP = function (a, b, n, m, results) {
    var t;

    t = ((a >>> n) ^ b) & m;
    a ^= t << n;
    b ^= t;

    results[0] = a;
    results[1] = b;
  };

  this.HPERM_OP = function (a, n, m) {
    var t;

    t = ((a << (16 - n)) ^ a) & m;
    a = a ^ t ^ (t >>> (16 - n));

    return (a);
  };

  this.des_set_key = function (key) {
    var schedule = new Array(this.ITERATIONS * 2);

    var c = this.fourBytesToInt(key, 0);
    var d = this.fourBytesToInt(key, 4);

    var results = new Array(2);

    this.PERM_OP(d, c, 4, 0x0f0f0f0f, results);
    d = results[0]; c = results[1];

    c = this.HPERM_OP(c, -2, 0xcccc0000);
    d = this.HPERM_OP(d, -2, 0xcccc0000);

    this.PERM_OP(d, c, 1, 0x55555555, results);
    d = results[0]; c = results[1];

    this.PERM_OP(c, d, 8, 0x00ff00ff, results);
    c = results[0]; d = results[1];

    this.PERM_OP(d, c, 1, 0x55555555, results);
    d = results[0]; c = results[1];

    d = (((d & 0x000000ff) << 16) | (d & 0x0000ff00) |
      ((d & 0x00ff0000) >>> 16) | ((c & 0xf0000000) >>> 4));
    c &= 0x0fffffff;

    var s, t;
    var j = 0;

    for (var i = 0; i < this.ITERATIONS; i++) {
      if (this.shifts2[i]) {
        c = (c >>> 2) | (c << 26);
        d = (d >>> 2) | (d << 26);
      } else {
        c = (c >>> 1) | (c << 27);
        d = (d >>> 1) | (d << 27);
      }

      c &= 0x0fffffff;
      d &= 0x0fffffff;

      s = this.skb[0][(c) & 0x3f] |
        this.skb[1][((c >>> 6) & 0x03) | ((c >>> 7) & 0x3c)] |
        this.skb[2][((c >>> 13) & 0x0f) | ((c >>> 14) & 0x30)] |
        this.skb[3][((c >>> 20) & 0x01) | ((c >>> 21) & 0x06) |
        ((c >>> 22) & 0x38)];

      t = this.skb[4][(d) & 0x3f] |
        this.skb[5][((d >>> 7) & 0x03) | ((d >>> 8) & 0x3c)] |
        this.skb[6][(d >>> 15) & 0x3f] |
        this.skb[7][((d >>> 21) & 0x0f) | ((d >>> 22) & 0x30)];

      schedule[j++] = ((t << 16) | (s & 0x0000ffff)) & 0xffffffff;
      s = ((s >>> 16) | (t & 0xffff0000));

      s = (s << 4) | (s >>> 28);
      schedule[j++] = s & 0xffffffff;
    }
    return schedule;
  };

  this.D_ENCRYPT = function (L, R, S, E0, E1, s) {
    var t, u, v;

    v = R ^ (R >>> 16);
    u = v & E0;
    v = v & E1;
    u = (u ^ (u << 16)) ^ R ^ s[S];
    t = (v ^ (v << 16)) ^ R ^ s[S + 1];
    t = (t >>> 4) | (t << 28);

    L ^= this.SPtrans[1][(t) & 0x3f] |
      this.SPtrans[3][(t >>> 8) & 0x3f] |
      this.SPtrans[5][(t >>> 16) & 0x3f] |
      this.SPtrans[7][(t >>> 24) & 0x3f] |
      this.SPtrans[0][(u) & 0x3f] |
      this.SPtrans[2][(u >>> 8) & 0x3f] |
      this.SPtrans[4][(u >>> 16) & 0x3f] |
      this.SPtrans[6][(u >>> 24) & 0x3f];

    return L;
  };

  this.body = function (schedule, Eswap0, Eswap1) {
    var left = 0;
    var right = 0;
    var t = 0;

    for (var j = 0; j < 25; j++) {
      for (var i = 0; i < this.ITERATIONS * 2; i += 4) {
        left = this.D_ENCRYPT(left, right, i, Eswap0, Eswap1, schedule);
        right = this.D_ENCRYPT(right, left, i + 2, Eswap0, Eswap1, schedule);
      }
      t = left;
      left = right;
      right = t;
    }

    t = right;

    right = (left >>> 1) | (left << 31);
    left = (t >>> 1) | (t << 31);

    left &= 0xffffffff;
    right &= 0xffffffff;

    var results = new Array(2);

    this.PERM_OP(right, left, 1, 0x55555555, results);
    right = results[0]; left = results[1];

    this.PERM_OP(left, right, 8, 0x00ff00ff, results);
    left = results[0]; right = results[1];

    this.PERM_OP(right, left, 2, 0x33333333, results);
    right = results[0]; left = results[1];

    this.PERM_OP(left, right, 16, 0x0000ffff, results);
    left = results[0]; right = results[1];

    this.PERM_OP(right, left, 4, 0x0f0f0f0f, results);
    right = results[0]; left = results[1];

    var out = new Array(2);

    out[0] = left; out[1] = right;

    return (out);
  };

  this.crypt = function (salt, original) {
    while (salt.length < 2) {
      salt += "A";
    }

    var buffer;
    var charZero = salt.charAt(0) + '';
    var charOne = salt.charAt(1) + '';
    var ccZ = charZero.charCodeAt(0);
    var ccO = charOne.charCodeAt(0);
    console.log("charZero", charZero, "charOne", charOne);

    buffer = charZero + charOne + " ";

    console.log("buffer \"" + buffer + "\"");
    var Eswap0 = this.con_salt[ccZ];
    var Eswap1 = this.con_salt[ccO] << 4;

    var key = [0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0];

    for (var i = 0; i < key.length && i < original.length; i++) {
      var iChar = original.charCodeAt(i);

      key[i] = iChar << 1;
    }

    var schedule = this.des_set_key(key);
    var out = this.body(schedule, Eswap0, Eswap1);

    var b = new Array(9);

    this.intToFourBytes(out[0], b, 0);
    this.intToFourBytes(out[1], b, 4);
    b[8] = 0;

    for (var i = 2, y = 0, u = 0x80; i < 13; i++) {
      for (var j = 0, c = 0; j < 6; j++) {
        c <<= 1;

        if ((b[y] & u) !== 0) {
          c |= 1;
        }

        u >>>= 1;

        if (u === 0) {
          y++;
          u = 0x80;
        }
        buffer = buffer.substring(0, i) + String.fromCharCode(this.cov_2char[c]) +
          buffer.substring(i + 1, buffer.length);
      }
    }
    return buffer;
  };
}
/** Crypt end **/

/** Pwr start **/

function PwrtStatus(sts) {
  this.sts = sts;

  this.evenSts = function () { return (sts % 2 === 0); };
  this.oddSts = function () { return (sts % 2 === 1); };
  this.getSts = function () { return sts; };
}

var Pwr = {
  eType_Boolean: 98305,
  eType_Float32: 98306,
  eType_Float64: 98307,
  eType_Char: 98308,
  eType_Int8: 98309,
  eType_Int16: 98310,
  eType_Int32: 98311,
  eType_UInt8: 98312,
  eType_UInt16: 98313,
  eType_UInt32: 98314,
  eType_Objid: 98315,
  eType_Buffer: 98316,
  eType_String: 98317,
  eType_Enum: 98318,
  eType_Struct: 98319,
  eType_Mask: 98320,
  eType_Array: 98321,
  eType_Time: 98322,
  eType_Text: 98323,
  eType_AttrRef: 98324,
  eType_UInt64: 98325,
  eType_Int64: 98326,
  eType_ClassId: 98327,
  eType_TypeId: 98328,
  eType_VolumeId: 98329,
  eType_ObjectIx: 98330,
  eType_RefId: 98331,
  eType_DeltaTime: 98332,
  eType_Status: 98333,
  eType_NetStatus: 98334,
  eType_CastId: 98335,
  eType_ProString: 98336,
  eType_DisableAttr: 98337,
  eType_DataRef: 98338,

  mPrv_RtRead: 1 << 0,
  mPrv_RtWrite: 1 << 1,
  mPrv_System: 1 << 2,
  mPrv_Maintenance: 1 << 3,
  mPrv_Process: 1 << 4,
  mPrv_Instrument: 1 << 5,
  mPrv_Operator1: 1 << 6,
  mPrv_Operator2: 1 << 7,
  mPrv_Operator3: 1 << 8,
  mPrv_Operator4: 1 << 9,
  mPrv_Operator5: 1 << 10,
  mPrv_Operator6: 1 << 11,
  mPrv_Operator7: 1 << 12,
  mPrv_Operator8: 1 << 13,
  mPrv_Operator9: 1 << 14,
  mPrv_Operator10: 1 << 15,
  mPrv_RtEventsAck: 1 << 18,
  mPrv_RtPlc: 1 << 19,
  mPrv_RtNavigator: 1 << 20,
  mPrv_DevRead: 1 << 21,
  mPrv_DevPlc: 1 << 22,
  mPrv_DevConfig: 1 << 23,
  mPrv_DevClass: 1 << 24,
  mPrv_RtEventsBlock: 1 << 25,
  mPrv_Administrator: 1 << 26,
  mPrv_SevRead: 1 << 27,
  mPrv_SevAdmin: 1 << 28,

  mAccess_RtRead: 1 << 0,
  mAccess_RtWrite: 1 << 1,
  mAccess_System: 1 << 2,
  mAccess_Maintenance: 1 << 3,
  mAccess_Process: 1 << 4,
  mAccess_Instrument: 1 << 5,
  mAccess_RtEventsBlock: 1 << 25,
  mAccess_RtEventsAck: 1 << 18,
  mAccess_RtPlc: 1 << 19,
  mAccess_RtNavigator: 1 << 20,
  mAccess_AllRt: 1 << 2 |
    1 << 3 |
    1 << 4 |
    1 << 5 |
    1 << 0 |
    1 << 1 |
    1 << 25 |
    1 << 18 |
    1 << 19 |
    1 << 20 |
    1 << 6 |
    1 << 7 |
    1 << 8 |
    1 << 9 |
    1 << 10 |
    1 << 11 |
    1 << 12 |
    1 << 13 |
    1 << 14 |
    1 << 15,
  mAccess_AllOperators: 1 << 6 |
    1 << 7 |
    1 << 8 |
    1 << 9 |
    1 << 10 |
    1 << 11 |
    1 << 12 |
    1 << 13 |
    1 << 14 |
    1 << 15,
  mAccess_AllPwr: ~0,

  mAdef_pointer: 1,
  mAdef_array: 2,
  mAdef_backup: 4,
  mAdef_changelog: 8,
  mAdef_state: 16,
  mAdef_const: 32,
  mAdef_rtvirtual: 64,
  mAdef_devbodyref: 128,
  mAdef_dynamic: 256,
  mAdef_publicwrite: 512,
  mAdef_noedit: 1024,
  mAdef_invisible: 2048,
  mAdef_refdirect: 4096,
  mAdef_noinvert: 8192,
  mAdef_noremove: 16384,
  mAdef_rtdbref: 32768,
  mAdef_private: 65536,
  mAdef_class: 131072,
  mAdef_superclass: 262144,
  mAdef_buffer: 524288,
  mAdef_nowbl: 1048576,
  mAdef_alwayswbl: 2097152,
  mAdef_disableattr: 4194304,
  mAdef_rthide: 8388608
};

var Pwrb = {
  mXttMethodsFlagsMask_IsConfigured: 1,

  mXttOpMethodsMask_OpenGraph: 1,
  mXttOpMethodsMask_OpenObjectGraph: 2,
  mXttOpMethodsMask_OpenTrend: 4,
  mXttOpMethodsMask_OpenHistory: 8,
  mXttOpMethodsMask_OpenFast: 16,
  mXttOpMethodsMask_Camera: 32,
  mXttOpMethodsMask_HistEvent: 64,
  mXttOpMethodsMask_BlockEvents: 128,
  mXttOpMethodsMask_Help: 256,
  mXttOpMethodsMask_Photo: 512,
  mXttOpMethodsMask_Note: 1024,
  mXttOpMethodsMask_ParentObjectGraph: 2048,

  mXttMntMethodsMask_OpenObject: 1,
  mXttMntMethodsMask_OpenTrace: 2,
  mXttMntMethodsMask_RtNavigator: 4,
  mXttMntMethodsMask_OpenCrossref: 8,
  mXttMntMethodsMask_HelpClass: 16,
  mXttMntMethodsMask_DataSheet: 32,
  mXttMntMethodsMask_CircuitDiagram: 64,
  mXttMntMethodsMask_Simulate: 1 << 31
};

function PwrtObjid(vid, oix) {
  this.oix = oix;
  this.vid = vid;
}

function CdhrNumber(value, sts) {
  this.value = value;
  this.sts = sts;

  this.evenSts = function () { return (sts % 2 === 0); };
  this.oddSts = function () { return (sts % 2 === 1); };
  this.getSts = function () { return sts; };
}

/** Start Gdh **/
function Sub() {
  this.sts;
  this.refid;
  this.type;
  this.elements;
  this.name;
  this.value;
}

function ObjectInfo() {
  this.objid;
  this.cid;
  this.has_children;
  this.name;
  this.description;
  this.classname;
  this.full_name;
}

function AttributeInfo() {
  this.name;
  this.type;
  this.size;
  this.flags;
  this.element;
  this.objid;
  this.full_name;
  this.classname;
}

function MenuButton() {
  this.type;
  this.text;
  this.name;
  this.url;
}

function OpwindMenuInfo() {
  this.title;
  this.text;
  this.enable_language;
  this.enable_login;
  this.enable_alarmlist;
  this.enable_eventlog;
  this.enable_navigator;
  this.disable_help;
  this.disable_proview;
  this.language;
  this.buttons = [];
}

function CrrInfo() {
  this.type;
  this.objid;
  this.name;
  this.classname;
}

function GlowPieInfo() {
  this.sector_num;
  this.min_val;
  this.max_val;
}

function GlowBarChartInfo() {
  this.bars;
  this.barsegments;
  this.min_value;
  this.max_value;
}

function GlowTableInfo() {
  this.columns;
  this.rows;
  this.column_size = new Array(Glow.TABLE_MAX_COL);
}

function PendingData(func_cb, data) {
  this.func_cb = func_cb;
  this.data = data;
}

var GdhOp = {
  GET_OP_SELF: 1,
  GET_OP_METHOD_PLC: 2
};

function Gdh() {
  var Msg = {
    SET_OBJECT_INFO_BOOLEAN: 1,
    SET_OBJECT_INFO_FLOAT: 2,
    SET_OBJECT_INFO_INT: 3,
    SET_OBJECT_INFO_STRING: 4,
    GET_OBJECT_INFO_BOOLEAN: 5,
    GET_OBJECT_INFO_FLOAT: 6,
    GET_OBJECT_INFO_INT: 7,
    GET_OBJECT_INFO_STRING: 8,
    TOGGLE_OBJECT_INFO: 9,
    REF_OBJECT_INFO: 10,
    GET_OBJECT_REF_INFO_BOOLEAN: 11,
    GET_OBJECT_REF_INFO_FLOAT: 12,
    GET_OBJECT_REF_INFO_INT: 13,
    GET_OBJECT_REF_INFO_STRING: 14,
    UNREF_OBJECT_INFO: 15,
    NAME_TO_OBJID: 16,
    OBJID_TO_NAME: 17,
    GET_ROOT_LIST: 18,
    GET_NEXT_OBJECT: 19,
    GET_CHILD: 20,
    GET_NEXT_SIBLING: 21,
    GET_OBJECT_CLASS: 22,
    GET_CLASS_LIST: 23,
    CLASS_ID_TO_OBJID: 24,
    GET_OBJECT_REF_INFO_ALL: 25,
    REF_OBJECT_INFO_LIST: 26,
    POLL: 27,
    STATISTICS: 28,
    CHECK_USER: 29,
    GET_NODE_OBJECT: 30,
    LOG_STRING: 31,
    UNREF_OBJECT_INFO_ALL: 32,
    CREATE_INSTANCE_FILE: 33,
    GET_ATTRIBUTE_CHAR: 34,
    GET_CLASS_ATTRIBUTE: 35,
    GET_ALL_CLASS_ATTRIBUTES: 36,
    GET_ALL_SIBLINGS: 37,
    GET_ALL_XTT_SIBLINGS: 38,
    GET_ALL_XTT_CHILDREN: 39,
    REF_OBJECT_INFO_VECTOR: 40,
    GET_SUBSCRIPTIONS: 41,
    CRR_SIGNAL: 42,
    CRR_OBJECT: 43,
    GET_PARENT: 44,
    GET_OBJECT_INFO_OBJID: 45,
    GET_OBJECT_REF_INFO_BOOLEAN_ARRAY: 46,
    GET_OBJECT_REF_INFO_FLOAT_ARRAY: 47,
    GET_OBJECT_REF_INFO_INT_ARRAY: 48,
    GET_OBJECT_REF_INFO_STRING_ARRAY: 49,
    GET_MSG: 50,
    GET_MSG_TEXT: 51,
    NAME_TO_ATTRREF: 52,
    ATTRREF_TO_NAME: 53,
    GET_ATTRREF_TID: 54,
    GET_SUPER_CLASS: 55,
    GET_ALL_CLASS_ATTRIBUTES_STRING: 56,
    GET_OBJECT_INFO_FLOAT_ARRAY: 57,
    GET_OBJECT_INFO_INT_ARRAY: 58,
    GET_CIRCBUFF_INFO: 59,
    UPDATE_CIRCBUFF_INFO: 60,
    GET_ATTRIBUTE_FLAGS: 61,
    CLASSNAME_TO_ID: 62,
    GET_OBJECT: 63,
    GET_OPWIND_MENU: 64
  };

  this.debug = false;
  this.pending = [];
  this.sub = [];
  this.PORT = 4448;
  this.ws = null;
  this.open_cb = null;
  this.close_cb = null;
  this.return_cb = null;
  this.next_id = 1234;
  this.subscriptionCount = 1;
  this.listSend = false;

  this.init = function () {
    if (window.location.hostname === "")
      this.ws = new WebSocket("ws:127.0.0.1:4448");
    else
      this.ws = new WebSocket("ws://" + window.location.hostname + ":4448");
    this.ws.binaryType = "arraybuffer";
    this.ws.gdh = this;

    this.ws.onopen = function (e) {
      if (this.gdh.open_cb !== null)
        this.gdh.open_cb();
    };

    this.ws.onclose = function () {
      if (this.debug) console.log("Socket closed");
      if (this.gdh.close_cb !== null)
        this.gdh.close_cb();
    };

    this.ws.onmessage = function (e) {
      if (typeof e.data === "string") {
        console.log("String message received", e, e.data);
      } else {
        if (e.data instanceof ArrayBuffer) {
          var dv = new DataView(e.data);
          var type = dv.getUint8(0);
          var id = dv.getUint32(1);
          var sts = dv.getUint32(5);

          switch (type) {
            case Msg.GET_OBJECT_INFO_BOOLEAN: {
              if (this.gdh.debug) console.log("GetObjectInfoBoolean received");
              var value = dv.getUint8(9);
              var func_cb = this.gdh.pending[id].func_cb;
              func_cb(id, sts, value);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.GET_OBJECT_INFO_INT: {
              if (this.gdh.debug) console.log("GetObjectInfoInt received");
              var value = dv.getUint32(9);
              var pending_data = this.gdh.pending[id];
              pending_data.func_cb(id, pending_data.data, sts, value);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.GET_OBJECT_INFO_FLOAT: {
              if (this.gdh.debug) console.log("GetObjectInfoFloat received");
              var value = dv.getFloat32(9);
              var pending_data = this.gdh.pending[id];
              pending_data.func_cb(id, pending_data.data, sts, value);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.GET_OBJECT_INFO_FLOAT_ARRAY: {
              if (this.gdh.debug) console.log("GetObjectInfoFloatArray received");
              var asize = dv.getInt32(9);
              var value = new Array(asize);
              k = 13;
              for (var i = 0; i < asize; i++) {
                value[i] = dv.getFloat32(k);
                k += 4;
              }
              var pending_data = this.gdh.pending[id];
              pending_data.func_cb(id, pending_data.data, sts, value);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.SET_OBJECT_INFO_BOOLEAN: {
              if (this.gdh.debug) console.log("SetObjectInfoBoolean received", id, sts);
              break;
            }
            case Msg.SET_OBJECT_INFO_INT: {
              if (this.gdh.debug) console.log("SetObjectInfoInt received", id, sts);
              break;
            }
            case Msg.SET_OBJECT_INFO_FLOAT: {
              if (this.gdh.debug) console.log("SetObjectInfoFloat received", id, sts);
              break;
            }
            case Msg.SET_OBJECT_INFO_STRING: {
              if (this.gdh.debug) console.log("SetObjectInfoString received", id, sts);
              break;
            }
            case Msg.TOGGLE_OBJECT_INFO: {
              if (this.gdh.debug) console.log("ToggleObjectInfo received", id, sts);
              break;
            }
            case Msg.REF_OBJECT_INFO: {
              if (this.gdh.debug) console.log("RefObjectInfo received", id, sts);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.UNREF_OBJECT_INFO: {
              if (this.gdh.debug) console.log("UnrefObjectInfo received", id, sts);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.REF_OBJECT_INFO_LIST: {
              if (this.gdh.debug) console.log("RefObjectInfoList received", id, sts);
              var func_cb = this.gdh.pending[id].func_cb;
              func_cb(id, sts);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.GET_OBJECT_REF_INFO_ALL: {
              var j = 9;
              var size = dv.getUint32(j);
              if (this.gdh.debug) console.log("GetObjectRefInfoAll received", id, size);
              j += 4;
              for (var i = 0; i < size; i++) {
                var eid = dv.getUint32(j);
                j += 4;
                var esize = dv.getUint32(j);
                j += 4;
                var sub = this.gdh.sub[eid];
                if (typeof sub === 'undefined')
                  j += esize;
                else {
                  var value;
                  switch (sub.type) {
                    case Pwr.eType_Boolean:
                      value = dv.getUint8(j);
                      j += 1;
                      break;
                    case Pwr.eType_Float32:
                      if (sub.elements <= 1) {
                        value = dv.getFloat32(j);
                        j += 4;
                      } else {
                        if (esize !== sub.elements * 4)
                          console.log("Subscription size error", esize, sub.elements, eid);
                        value = new Array(sub.elements);
                        for (var k = 0; k < sub.elements; k++) {
                          value[k] = dv.getFloat32(j);
                          j += 4;
                        }
                      }
                      break;
                    case Pwr.eType_Int8:
                    case Pwr.eType_Int16:
                    case Pwr.eType_Int32:
                    case Pwr.eType_UInt8:
                    case Pwr.eType_UInt16:
                    case Pwr.eType_UInt32:
                    case Pwr.eType_Status:
                    case Pwr.eType_Mask:
                    case Pwr.eType_Enum:
                      value = dv.getInt32(j);
                      j += 4;
                      break;
                    case Pwr.eType_String:
                    case Pwr.eType_Time:
                    case Pwr.eType_DeltaTime:
                    case Pwr.eType_AttrRef:
                    case Pwr.eType_Objid:
                      var nsize = dv.getInt16(j);
                      j += 2;
                      var iarr = new Uint8Array(nsize);
                      for (var k = 0; k < nsize; k++) {
                        iarr[k] = dv.getUint8(j++);
                      }
                      value = String.fromCharCode.apply(null, iarr);
                      break;
                    default: break;
                  }
                  this.gdh.sub[eid].value = value;
                }
              }
              if (typeof this.gdh.pending[id] === 'undefined') {
                console.log("** GetObjectRefInfoAll received removed", id);
                break;
              }
              var func_cb = this.gdh.pending[id].func_cb;
              func_cb(id, sts);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.GET_ALL_XTT_CHILDREN: {
              var result = [];
              var j = 9;
              var size = dv.getUint32(j);
              if (this.gdh.debug) console.log("GetAllXttChildren received", id, size);
              console.log("GetAllXttChildren received", sts, id, size);
              j += 4;
              for (var i = 0; i < size; i++) {
                var info = new ObjectInfo();
                info.objid = new PwrtObjid();
                info.objid.vid = dv.getUint32(j);
                j += 4;
                info.objid.oix = dv.getUint32(j);
                j += 4;
                info.cid = dv.getUint32(j);
                j += 4;
                info.has_children = dv.getUint16(j) !== 0;
                j += 2;
                var nsize = dv.getUint16(j);
                j += 2;
                var iarr = new Uint8Array(nsize);
                for (var k = 0; k < nsize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                info.name = String.fromCharCode.apply(null, iarr);
                //j += nsize;
                var dsize = dv.getUint16(j);
                j += 2;
                iarr = new Uint8Array(dsize);
                for (var k = 0; k < dsize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                info.description = String.fromCharCode.apply(null, iarr);
                var csize = dv.getUint16(j);
                j += 2;
                iarr = new Uint8Array(csize);
                for (var k = 0; k < csize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                info.classname = String.fromCharCode.apply(null, iarr);
                result.push(info);
              }
              var pending_data = this.gdh.pending[id];
              pending_data.func_cb(id, pending_data.data, sts, result);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.GET_ALL_CLASS_ATTRIBUTES: {
              var result = [];
              var j = 9;
              var size = dv.getUint32(j);
              if (this.gdh.debug) console.log("GetAllClassAttributes received", id, size);
              j += 4;
              for (var i = 0; i < size; i++) {
                var info = new AttributeInfo();
                info.type = dv.getUint32(j);
                j += 4;
                info.flags = dv.getUint32(j);
                j += 4;
                info.size = dv.getUint16(j);
                j += 2;
                info.elements = dv.getUint16(j);
                j += 2;
                var nsize = dv.getUint16(j);
                j += 2;
                var iarr = new Uint8Array(nsize);
                for (var k = 0; k < nsize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                info.name = String.fromCharCode.apply(null, iarr);

                nsize = dv.getUint16(j);
                j += 2;
                iarr = new Uint8Array(nsize);
                for (var k = 0; k < nsize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                info.classname = String.fromCharCode.apply(null, iarr);
                result.push(info);
              }
              var pending_data = this.gdh.pending[id];
              pending_data.func_cb(id, pending_data.data, sts, result);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.GET_OBJECT: {
              if (this.gdh.debug) console.log("GetObject received", id, sts);
              var info = null;
              if ((sts & 1) !== 0) {
                var j = 9;
                info = new ObjectInfo();
                info.objid = new PwrtObjid();
                info.objid.vid = dv.getUint32(j);
                j += 4;
                info.objid.oix = dv.getUint32(j);
                j += 4;
                info.cid = dv.getUint32(j);
                j += 4;
                info.has_children = dv.getUint16(j) !== 0;
                j += 2;

                var nsize = dv.getUint16(j);
                j += 2;
                var iarr = new Uint8Array(nsize);
                for (var k = 0; k < nsize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                info.name = String.fromCharCode.apply(null, iarr);

                nsize = dv.getUint16(j);
                j += 2;
                iarr = new Uint8Array(nsize);
                for (var k = 0; k < nsize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                info.fullname = String.fromCharCode.apply(null, iarr);

                var csize = dv.getUint16(j);
                j += 2;
                iarr = new Uint8Array(csize);
                for (var k = 0; k < csize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                info.classname = String.fromCharCode.apply(null, iarr);

                var dsize = dv.getUint16(j);
                j += 2;
                iarr = new Uint8Array(dsize);
                for (var k = 0; k < dsize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                info.description = String.fromCharCode.apply(null, iarr);
              }
              var pending_data = this.gdh.pending[id];
              pending_data.func_cb(id, pending_data.data, sts, info);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.CRR_SIGNAL: {
              var crrtext = null;
              if ((sts & 1) !== 0) {
                var j = 9;
                var result = [];
                var size = dv.getUint16(j);
                j += 2;
                if (this.gdh.debug) console.log("CrrSignal received", id, size);
                for (var i = 0; i < size; i++) {
                  var info = new CrrInfo();
                  info.type = dv.getUint16(j);
                  j += 2;
                  info.objid = new PwrtObjid();
                  info.objid.vid = dv.getUint32(j);
                  j += 4;
                  info.objid.oix = dv.getUint32(j);
                  j += 4;
                  var nsize = dv.getUint16(j);
                  j += 2;
                  var iarr = new Uint8Array(nsize);
                  for (var k = 0; k < nsize; k++) {
                    iarr[k] = dv.getUint8(j++);
                  }
                  info.name = String.fromCharCode.apply(null, iarr);
                  var csize = dv.getUint16(j);
                  j += 2;
                  iarr = new Uint8Array(csize);
                  for (var k = 0; k < csize; k++) {
                    iarr[k] = dv.getUint8(j++);
                  }
                  info.classname = String.fromCharCode.apply(null, iarr);
                  result.push(info);
                }
              }
              var pending_data = this.gdh.pending[id];
              pending_data.func_cb(id, pending_data.data, sts, result);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.GET_OPWIND_MENU: {
              var result = new OpwindMenuInfo();
              var j = 9;
              if (this.gdh.debug) console.log("GetOpwindMenu received", id, size);
              console.log("GetOpwindMenu received", sts, id);

              if (sts & 1) {
                var nsize = dv.getUint16(j);
                j += 2;
                var iarr = new Uint8Array(nsize);
                for (var k = 0; k < nsize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                result.title = String.fromCharCode.apply(null, iarr);

                nsize = dv.getUint16(j);
                j += 2;
                iarr = new Uint8Array(nsize);
                for (var k = 0; k < nsize; k++) {
                  iarr[k] = dv.getUint8(j++);
                }
                result.text = String.fromCharCode.apply(null, iarr);

                result.enable_language = dv.getUint32(j);
                j += 4;
                result.enable_login = dv.getUint32(j);
                j += 4;
                result.enable_alarmlist = dv.getUint32(j);
                j += 4;
                result.enable_eventlog = dv.getUint32(j);
                j += 4;
                result.enable_navigator = dv.getUint32(j);
                j += 4;
                result.disable_help = dv.getUint32(j);
                j += 4;
                result.disable_proview = dv.getUint32(j);
                j += 4;
                result.language = dv.getUint32(j);
                j += 4;

                var bsize = dv.getUint16(j);
                j += 2;

                for (var i = 0; i < bsize; i++) {
                  var button = new MenuButton();
                  button.type = dv.getUint32(j);
                  j += 4;
                  nsize = dv.getUint16(j);
                  j += 2;
                  iarr = new Uint8Array(nsize);
                  for (var k = 0; k < nsize; k++) {
                    iarr[k] = dv.getUint8(j++);
                  }
                  button.text = String.fromCharCode.apply(null, iarr);
                  nsize = dv.getUint16(j);
                  j += 2;
                  iarr = new Uint8Array(nsize);
                  for (var k = 0; k < nsize; k++) {
                    iarr[k] = dv.getUint8(j++);
                  }
                  button.name = String.fromCharCode.apply(null, iarr);
                  nsize = dv.getUint16(j);
                  j += 2;
                  iarr = new Uint8Array(nsize);
                  for (var k = 0; k < nsize; k++) {
                    iarr[k] = dv.getUint8(j++);
                  }
                  button.url = String.fromCharCode.apply(null, iarr);
                  result.buttons.push(button);
                }
              }
              var pending_data = this.gdh.pending[id];
              pending_data.func_cb(id, pending_data.data, sts, result);
              delete this.gdh.pending[id];
              break;
            }
            case Msg.CHECK_USER: {
              var j = 9;
              if (this.gdh.debug) console.log("Check user received", id, size);
              console.log("Check user received", sts, id);

              var priv = 0;
              if (sts & 1) {
                priv = dv.getUint32(j);
                j += 4;
              }
              var pending_data = this.gdh.pending[id];
              pending_data.func_cb(id, pending_data.data, sts, priv);
              delete this.gdh.pending[id];
              break;
            }
            default:
              console.log("Unknown message type");
          }
        }
      }
    };
  };

  this.getObjectInfoBoolean = function (name, return_cb) {
    this.return_cb = return_cb;

    var buf = new Uint8Array(name.length + 6);
    buf[0] = Msg.GET_OBJECT_INFO_BOOLEAN;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    for (var i = 0; i < name.length; i++) {
      buf[i + 6] = name.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData(return_cb, null);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getObjectInfoInt = function (name, return_cb, data) {
    this.return_cb = return_cb;

    var buf = new Uint8Array(name.length + 6);
    buf[0] = Msg.GET_OBJECT_INFO_INT;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    for (var i = 0; i < name.length; i++) {
      buf[i + 6] = name.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData(return_cb, data);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getObjectInfoFloat = function (name, return_cb, data) {
    this.return_cb = return_cb;

    var buf = new Uint8Array(name.length + 6);
    buf[0] = Msg.GET_OBJECT_INFO_FLOAT;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    for (var i = 0; i < name.length; i++) {
      buf[i + 6] = name.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData(return_cb, data);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getObjectInfoFloatArray = function (name, asize, return_cb, data) {
    this.return_cb = return_cb;

    var buf = new Uint8Array(name.length + 10);
    buf[0] = Msg.GET_OBJECT_INFO_FLOAT_ARRAY;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = asize & 0xFF;
    buf[7] = (asize >> 8) & 0xFF;
    buf[8] = (asize >> 16) & 0xFF;
    buf[9] = (asize >> 24) & 0xFF;
    for (var i = 0; i < name.length; i++) {
      buf[i + 10] = name.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData(return_cb, data);
    this.ws.send(buf);
    this.next_id++;
  };

  this.refObjectInfo = function (name, type, elements) {
    var sub = new Sub();
    sub.name = name;
    sub.refid = this.subscriptionCount;
    sub.type = type;
    sub.elements = elements;
    this.sub[this.subscriptionCount] = sub;
    this.subscriptionCount++;
    if (!this.listSent) {
      return sub.refid;
    } else {
      var len = 0;

      var size = 12 + sub.name.length;

      var buf = new Uint8Array(size + 10);
      buf[0] = Msg.REF_OBJECT_INFO;
      buf[2] = this.next_id & 0xFF;
      buf[3] = (this.next_id >> 8) & 0xFF;
      buf[4] = (this.next_id >> 16) & 0xFF;
      buf[5] = (this.next_id >> 24) & 0xFF;
      if (this.debug) console.log("RefObjectInfo: ", sub.refid);
      var k = 6;
      buf[k++] = sub.refid & 0xFF;
      buf[k++] = (sub.refid >> 8) & 0xFF;
      buf[k++] = (sub.refid >> 16) & 0xFF;
      buf[k++] = (sub.refid >> 24) & 0xFF;
      buf[k++] = sub.elements & 0xFF;
      buf[k++] = (sub.elements >> 8) & 0xFF;
      buf[k++] = (sub.elements >> 16) & 0xFF;
      buf[k++] = (sub.elements >> 24) & 0xFF;
      buf[k++] = sub.name.length & 0xFF;
      buf[k++] = (sub.name.length >> 8) & 0xFF;
      buf[k++] = (sub.name.length >> 16) & 0xFF;
      buf[k++] = (sub.name.length >> 24) & 0xFF;

      for (var j = 0; j < sub.name.length; j++) {
        buf[k++] = sub.name.charCodeAt(j);
      }

      this.pending[this.next_id] = new PendingData(this.refObjectInfoReply, null);
      if (this.debug) console.log("Sending RefObjectInfo", this.next_id, size, k);
      this.ws.send(buf);

      this.next_id++;

      return sub.refid;
    }
  };
  this.refObjectInfoReply = function (id, sts) {
    if (this.debug) console.log("refObjectInfoReply", id, sts);
  };
  this.unrefObjectInfo = function (refid) {
    var len = 0;

    var size = 4;

    var buf = new Uint8Array(size + 10);
    buf[0] = Msg.UNREF_OBJECT_INFO;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    if (this.debug) console.log("UnrefObjectInfo: ", refid);
    var k = 6;
    buf[k++] = refid & 0xFF;
    buf[k++] = (refid >> 8) & 0xFF;
    buf[k++] = (refid >> 16) & 0xFF;
    buf[k++] = (refid >> 24) & 0xFF;

    this.pending[this.next_id] = new PendingData(this.unrefObjectInfoReply, null);
    if (this.debug) console.log("Sending UnrefObjectInfo", this.next_id, size, k, refid);
    console.log("Sending UnrefObjectInfo", this.next_id, size, k, refid);
    this.ws.send(buf);

    this.next_id++;
    delete this.sub[refid];
  };
  this.refObjectInfoList = function (return_cb) {
    var size = 0;
    var len = 0;

    this.return_cb = return_cb;

    for (var i in this.sub) {
      size += 12 + this.sub[i].name.length;
      len++;
    }
    var buf = new Uint8Array(size + 10);
    buf[0] = Msg.REF_OBJECT_INFO_LIST;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = len & 0xFF;
    buf[7] = (len >> 8) & 0xFF;
    buf[8] = (len >> 16) & 0xFF;
    buf[9] = (len >> 24) & 0xFF;
    var k = 10;
    for (var i in this.sub) {
      if (i === 0)
        continue;
      if (this.debug) console.log("RefObjectInfoList: ", this.sub[i].refid);
      buf[k++] = this.sub[i].refid & 0xFF;
      buf[k++] = (this.sub[i].refid >> 8) & 0xFF;
      buf[k++] = (this.sub[i].refid >> 16) & 0xFF;
      buf[k++] = (this.sub[i].refid >> 24) & 0xFF;
      buf[k++] = this.sub[i].elements & 0xFF;
      buf[k++] = (this.sub[i].elements >> 8) & 0xFF;
      buf[k++] = (this.sub[i].elements >> 16) & 0xFF;
      buf[k++] = (this.sub[i].elements >> 24) & 0xFF;
      buf[k++] = this.sub[i].name.length & 0xFF;
      buf[k++] = (this.sub[i].name.length >> 8) & 0xFF;
      buf[k++] = (this.sub[i].name.length >> 16) & 0xFF;
      buf[k++] = (this.sub[i].name.length >> 24) & 0xFF;

      for (var j = 0; j < this.sub[i].name.length; j++) {
        buf[k++] = this.sub[i].name.charCodeAt(j);
      }
    }
    this.pending[this.next_id] = new PendingData(return_cb, null);
    if (this.debug) console.log("Sending RefObjectInfoList", this.next_id, size, k, this.next_id);
    this.ws.send(buf);

    this.next_id++;
    this.listSent = true;
  };
  this.refObjectInfoListReply = function (id, sts) {
    if (this.debug) console.log("refObjectInfoListReply", id, sts);
  };
  this.getRefObjectInfoAll = function (return_cb) {
    var buf = new Uint8Array(6);
    buf[0] = Msg.GET_OBJECT_REF_INFO_ALL;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    this.pending[this.next_id] = new PendingData(return_cb, null);
    if (this.debug) console.log("Sending getRefObjectInfoAll", this.next_id);
    this.ws.send(buf);
    this.next_id++;
  };
  this.getRefObjectInfoAllReply = function (id, sts) {
    if (this.debug) console.log("getRefObjectInfoAllReply", id, sts);
  };
  this.getObjectRefInfo = function (id) {
    if (this.debug) console.log("getObjectRefInfo", id, this.sub[id].value);
    return this.sub[id].value;
  };
  this.setObjectInfoBoolean = function (name, value) {
    var buf = new Uint8Array(12 + name.length);
    buf[0] = Msg.SET_OBJECT_INFO_BOOLEAN;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = value & 0xFF;
    buf[7] = (value >> 8) & 0xFF;
    buf[8] = (value >> 16) & 0xFF;
    buf[9] = (value >> 24) & 0xFF;
    buf[10] = name.length & 0xFF;
    buf[11] = (name.length >> 8) & 0xFF;
    var k = 12;
    for (var i = 0; i < name.length; i++) {
      buf[k++] = name.charCodeAt(i);
    }
    this.ws.send(buf);
    if (this.debug) console.log("Sending setObjectInfoBoolean", this.next_id, name, value);
    this.next_id++;

    return new PwrtStatus(1);
  };

  this.setObjectInfoInt = function (name, value) {
    var buf = new Uint8Array(12 + name.length);
    buf[0] = Msg.SET_OBJECT_INFO_INT;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = value & 0xFF;
    buf[7] = (value >> 8) & 0xFF;
    buf[8] = (value >> 16) & 0xFF;
    buf[9] = (value >> 24) & 0xFF;
    buf[10] = name.length & 0xFF;
    buf[11] = (name.length >> 8) & 0xFF;
    var k = 12;
    for (var i = 0; i < name.length; i++) {
      buf[k++] = name.charCodeAt(i);
    }
    // this.pending[this.next_id] = new PendingData( return_cb, null);
    this.ws.send(buf);
    if (this.debug) console.log("Sending setObjectInfoInt", this.next_id, name, value);
    this.next_id++;

    return new PwrtStatus(1);
  };

  this.setObjectInfoFloat = function (name, value) {
    var buf = new Uint8Array(12 + name.length);
    buf[0] = Msg.SET_OBJECT_INFO_FLOAT;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    var fbuf = new ArrayBuffer(4);
    var fa = new Float32Array(fbuf);
    fa[0] = value;
    var ba = new Uint8Array(fbuf);
    buf[6] = ba[0];
    buf[7] = ba[1];
    buf[8] = ba[2];
    buf[9] = ba[3];
    buf[10] = name.length & 0xFF;
    buf[11] = (name.length >> 8) & 0xFF;
    var k = 12;
    for (var i = 0; i < name.length; i++) {
      buf[k++] = name.charCodeAt(i);
    }
    // this.pending[this.next_id] = new PendingData( return_cb, null);
    this.ws.send(buf);
    if (this.debug) console.log("Sending setObjectInfoFloat", this.next_id, name, value);
    this.next_id++;

    return new PwrtStatus(1);
  };

  this.setObjectInfoString = function (name, value) {
    var i;
    var buf = new Uint8Array(10 + value.length + name.length);
    buf[0] = Msg.SET_OBJECT_INFO_STRING;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = value.length & 0xFF;
    buf[7] = (value.length >> 8) & 0xFF;
    var k = 8;
    for (i = 0; i < value.length; i++)
      buf[k++] = value.charCodeAt(i);
    buf[k++] = name.length & 0xFF;
    buf[k++] = (name.length >> 8) & 0xFF;
    for (i = 0; i < name.length; i++) {
      buf[k++] = name.charCodeAt(i);
    }
    // this.pending[this.next_id] = new PendingData( return_cb, null);
    this.ws.send(buf);
    if (this.debug) console.log("Sending setObjectInfoString", this.next_id, name, value);
    this.next_id++;

    return new PwrtStatus(1);
  };

  this.toggleObjectInfo = function (name) {
    var buf = new Uint8Array(8 + name.length);
    buf[0] = Msg.TOGGLE_OBJECT_INFO;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = name.length & 0xFF;
    buf[7] = (name.length >> 8) & 0xFF;
    var k = 8;
    for (var i = 0; i < name.length; i++) {
      buf[k++] = name.charCodeAt(i);
    }
    // this.pending[this.next_id] = new PendingData( return_cb, null);
    this.ws.send(buf);
    if (this.debug) console.log("Sending toggleObjectInfoBoolean", this.next_id, name, value);
    this.next_id++;

    return new PwrtStatus(1);
  };

  this.getAllXttChildren = function (oid, return_cb, data) {
    var buf = new Uint8Array(14);
    buf[0] = Msg.GET_ALL_XTT_CHILDREN;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = oid.vid & 0xFF;
    buf[7] = (oid.vid >> 8) & 0xFF;
    buf[8] = (oid.vid >> 16) & 0xFF;
    buf[9] = (oid.vid >> 24) & 0xFF;
    buf[10] = oid.oix & 0xFF;
    buf[11] = (oid.oix >> 8) & 0xFF;
    buf[12] = (oid.oix >> 16) & 0xFF;
    buf[13] = (oid.oix >> 24) & 0xFF;
    this.pending[this.next_id] = new PendingData(return_cb, data);
    if (this.debug) console.log("Sending getAllXttChildren", this.next_id);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getAllClassAttributes = function (cid, oid, return_cb, data) {
    var buf = new Uint8Array(18);
    buf[0] = Msg.GET_ALL_CLASS_ATTRIBUTES;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = cid & 0xFF;
    buf[7] = (cid >> 8) & 0xFF;
    buf[8] = (cid >> 16) & 0xFF;
    buf[9] = (cid >> 24) & 0xFF;
    buf[10] = oid.vid & 0xFF;
    buf[11] = (oid.vid >> 8) & 0xFF;
    buf[12] = (oid.vid >> 16) & 0xFF;
    buf[13] = (oid.vid >> 24) & 0xFF;
    buf[14] = oid.oix & 0xFF;
    buf[15] = (oid.oix >> 8) & 0xFF;
    buf[16] = (oid.oix >> 16) & 0xFF;
    buf[17] = (oid.oix >> 24) & 0xFF;
    this.pending[this.next_id] = new PendingData(return_cb, data);
    if (this.debug) console.log("Sending getAllClassAttributes", this.next_id, cid, oid.vid, oid.oix);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getObject = function (oid, op, return_cb, data) {
    var buf = new Uint8Array(16);
    buf[0] = Msg.GET_OBJECT;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = op & 0xFF;
    buf[7] = (op >> 8) & 0xFF;
    buf[8] = oid.vid & 0xFF;
    buf[9] = (oid.vid >> 8) & 0xFF;
    buf[10] = (oid.vid >> 16) & 0xFF;
    buf[11] = (oid.vid >> 24) & 0xFF;
    buf[12] = oid.oix & 0xFF;
    buf[13] = (oid.oix >> 8) & 0xFF;
    buf[14] = (oid.oix >> 16) & 0xFF;
    buf[15] = (oid.oix >> 24) & 0xFF;
    this.pending[this.next_id] = new PendingData(return_cb, data);
    if (this.debug) console.log("Sending getObject", this.next_id, oid.vid, oid.oix);
    this.ws.send(buf);
    this.next_id++;
  };

  this.crrSignal = function (oid, return_cb, data) {
    var buf = new Uint8Array(14);
    buf[0] = Msg.CRR_SIGNAL;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = oid.vid & 0xFF;
    buf[7] = (oid.vid >> 8) & 0xFF;
    buf[8] = (oid.vid >> 16) & 0xFF;
    buf[9] = (oid.vid >> 24) & 0xFF;
    buf[10] = oid.oix & 0xFF;
    buf[11] = (oid.oix >> 8) & 0xFF;
    buf[12] = (oid.oix >> 16) & 0xFF;
    buf[13] = (oid.oix >> 24) & 0xFF;
    this.pending[this.next_id] = new PendingData(return_cb, data);
    if (this.debug) console.log("Sending crrObject", this.next_id, oid.vid, oid.oix);
    this.ws.send(buf);
    this.next_id++;
  };

  this.getOpwindMenu = function (name, return_cb, data) {
    var len = name.length;

    var buf = new Uint8Array(8 + name.length);
    buf[0] = Msg.GET_OPWIND_MENU;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    buf[6] = name.length & 0xFF;
    buf[7] = (name.length >> 8) & 0xFF;
    var k = 8;
    for (var i = 0; i < name.length; i++) {
      buf[k++] = name.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData(return_cb, data);
    if (this.debug) console.log("Sending getOpwindMenu", this.next_id);
    this.ws.send(buf);
    this.next_id++;
  };

  this.login = function (user, passwd, return_cb, data) {
    var buf = new Uint8Array(6 + 2 + user.length + 2 + passwd.length);
    buf[0] = Msg.CHECK_USER;
    buf[2] = this.next_id & 0xFF;
    buf[3] = (this.next_id >> 8) & 0xFF;
    buf[4] = (this.next_id >> 16) & 0xFF;
    buf[5] = (this.next_id >> 24) & 0xFF;
    var k = 6;
    buf[k] = user.length & 0xFF;
    buf[k + 1] = (user.length >> 8) & 0xFF;
    k += 2;
    for (var i = 0; i < user.length; i++) {
      buf[k++] = user.charCodeAt(i);
    }
    buf[k] = passwd.length & 0xFF;
    buf[k + 1] = (passwd.length >> 8) & 0xFF;
    k += 2;
    for (var i = 0; i < passwd.length; i++) {
      buf[k++] = passwd.charCodeAt(i);
    }
    this.pending[this.next_id] = new PendingData(return_cb, data);
    if (this.debug) console.log("Sending login", this.next_id);
    this.ws.send(buf);
    this.next_id++;
  };

}
/** End Gdh **/

/** Start OpWind **/

function OpWindMenu() {
  this.gdh = 0;
  this.buttons = null;
  this.priv = 0;
  this.user = "";
  this.user_text = null;
  this.host;

  var self = this;

  this.init = function () {
    this.host = window.location.hostname;
    if (this.host === "")
      this.host = "localhost";

    this.gdh = new Gdh();
    this.gdh.open_cb = this.gdh_init_cb;
    this.gdh.init();
  };

  this.is_authorized = function (access) {
    return !!(this.priv & access);
  };

  this.get_opplace = function () {
    var query = window.location.search.substring(1);

    console.log("query", query);
    var vars = query.split('&');
    console.log("vars", vars.length, vars[0].substring(8));
    return vars[0].substring(8);
  };

  this.gdh_init_cb = function () {
    var oid = new PwrtObjid(0, 0);
    self.user = "Default";
    self.gdh.login("", "", self.login_cb, self);
    self.gdh.getOpwindMenu(self.get_opplace(), self.get_menu_cb, 999);

  };

  this.add_menu_button = function (context, text) {
    var button = document.createElement("input");
    button.type = "button";
    button.className = "leftmenu-button";
    button.value = text;
    button.addEventListener('click', new Function("menu.button_cb(\"" + button.value + "\")"));
    context.appendChild(button);
    context.appendChild(document.createElement("br"));
    return button;
  };

  this.get_menu_cb = function (id, data, sts, result) {
    self.info = result;
    console.log("Menu received", sts, data, result.buttons.length);
    var context = document.getElementById("opwindmenu");

    document.getElementById("opwind_title").innerHTML = result.title;
    document.getElementById("opwind_text").innerHTML = result.text;

    if (result.enable_login) {
      self.user_text = document.createTextNode(self.user + " on " + self.host);
      context.appendChild(self.user_text);
      context.appendChild(document.createElement("hr"));

      document.getElementById("login_button").addEventListener("click", function (event) {
        if (document.getElementById("login_frame").style.visibility === 'hidden') {
          document.getElementById("login_user").value = "";
          document.getElementById("login_passw").value = "";
          document.getElementById("login_frame").style.visibility = 'visible';
          document.getElementById("login_frame").style.height = '120px';
          document.getElementById("login_user").focus();
        } else {
          document.getElementById("login_frame").style.visibility = 'hidden';
          document.getElementById("login_frame").style.height = '0px';
        }
      });
      document.getElementById("apply_button").addEventListener("click", function (event) {
        var user = document.getElementById("login_user").value;
        var passwd = document.getElementById("login_passw").value;
        if (user.trim() === "")
          return;
        document.getElementById("login_frame").style.visibility = 'hidden';
        document.getElementById("login_frame").style.height = '0px';
        var c = new JopCrypt();
        passwd = c.crypt("aa", passwd);

        self.user = user;
        self.gdh.login(user, passwd, self.login_cb, self);
      });
      document.getElementById("cancel_button").addEventListener("click", function (event) {
        document.getElementById("login_frame").style.visibility = 'hidden';
        document.getElementById("login_frame").style.height = '0px';
      });
      document.getElementById("logout_button").addEventListener("click", function (event) {
        document.getElementById("login_frame").style.visibility = 'hidden';
        document.getElementById("login_frame").style.height = '0px';
        self.priv = 0;
        self.user = "Default";
        self.gdh.login("", "", self.login_cb, self);
      });

      document.getElementById("login_user").value = "";
      document.getElementById("login_passw").value = "";
      //document.getElementById("login_frame").setAttribute("style", "visibility:hidden;height:10px";
      document.getElementById("login_frame").style.visibility = 'hidden';
      document.getElementById("login_frame").style.height = '0px';
    } else {
      document.getElementById("login_button").remove();
      document.getElementById("login_frame").remove();
    }
    if (result.enable_language)
      self.add_menu_button(context, "Language");
    if (result.enable_alarmlist)
      self.add_menu_button(context, "AlarmList");
    if (result.enable_eventlog)
      self.add_menu_button(context, "EventLog");
    if (result.enable_navigator)
      self.add_menu_button(context, "Navigator");
    if (!result.disable_help)
      self.add_menu_button(context, "Help");
    if (!result.disable_proview)
      self.add_menu_button(context, "ProviewR");

    context.appendChild(document.createElement("hr"));

    var button;
    for (var i = 0; i < result.buttons.length; i++) {
      self.add_menu_button(context, result.buttons[i].text);
    }
  };

  this.button_cb = function (text) {
    if (self.info.enable_language && text === "Language") {
      console.log("Language activated");
    } else if (self.info.enable_alarmlist && text === "AlarmList") {
      console.log("AlarmList activated");
      if (!(self.is_authorized(Pwr.mAccess_RtRead | Pwr.mAccess_RtWrite |
        Pwr.mAccess_AllOperators |
        Pwr.mAccess_System | Pwr.mAccess_Maintenance |
        Pwr.mAccess_Process | Pwr.mAccess_Instrument)))
        window.alert("Not authorized for this operation");
      else
        window.alert("Not yet implemented");
    } else if (self.info.enable_eventlog && text === "EventLog") {
      console.log("EventLog activated");
      if (!(self.is_authorized(Pwr.mAccess_RtRead | Pwr.mAccess_RtWrite |
        Pwr.mAccess_AllOperators |
        Pwr.mAccess_System | Pwr.mAccess_Maintenance |
        Pwr.mAccess_Process | Pwr.mAccess_Instrument)))

        window.alert("Not authorized for this operation");
      else
        window.alert("Not yet implemented");
    } else if (self.info.enable_navigator && text === "Navigator") {
      console.log("Navigator activated");
      if (!(self.is_authorized(Pwr.mAccess_RtNavigator |
        Pwr.mAccess_System | Pwr.mAccess_Maintenance |
        Pwr.mAccess_Process | Pwr.mAccess_Instrument)))
        window.alert("Not authorized for this operation");
      else
        window.open("xtt.html", "_blank");
    } else if (!self.info.disable_help && text === "Help") {
      console.log("Help activated");
      window.open("xtt_help_index.html", "_blank");
    } else if (!self.info.disable_proview && text === "ProviewR") {
      console.log("ProviewR activated");
      window.open("http://www.proview.se", "_blank");
    } else {
      if (!(self.is_authorized(Pwr.mAccess_RtRead | Pwr.mAccess_RtWrite |
        Pwr.mAccess_AllOperators |
        Pwr.mAccess_System | Pwr.mAccess_Maintenance |
        Pwr.mAccess_Process | Pwr.mAccess_Instrument)))
        window.alert("Not authorized for this operation");
      else {
        for (var i = 0; i < self.info.buttons.length; i++) {
          if (self.info.buttons[i].text === text) {
            console.log("Found", self.info.buttons[i].text);
            var name = self.info.buttons[i].name;
            var n = name.indexOf(".pwg");
            if (n !== -1)
              name = name.substring(0, n);
            var url = "ge.html?graph=" + name;
            console.log("url", url);
            window.open(url, "_blank");
            break;
          }
        }
      }
    }
  };

  this.login_cb = function (id, data, sts, result) {
    console.log("Login:", sts, result);
    if (sts & 1) {
      self.priv = result;
      sessionStorage.setItem("pwr_privilege", self.priv);
      if (self.user_text != null)
        self.user_text.textContent = self.user + " on " + self.host;

      console.log("Login", self.user, "Priv", self.priv);
    } else {
      self.priv = 0;
      self.user = "none";
      sessionStorage.setItem("pwr_privilege", self.priv);
      if (self.user_text != null)
        self.user_text.textContent = "None on " + self.host;
      console.log("Login failure", "Priv", self.priv);
    }
  };
}

var menu = new OpWindMenu();
menu.init();

/** End OpWind **/
