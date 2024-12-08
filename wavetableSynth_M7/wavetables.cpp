#include "wavetables.h"
#include <Arduino.h>  // Included to make uint16_t available

uint16_t sine_wave[512] = {
    0x7FF, 0x818, 0x831, 0x84A, 0x863, 0x87D, 0x896, 0x8AF, 0x8C8, 0x8E1, 0x8FA, 0x913, 
    0x92B, 0x944, 0x95D, 0x976, 0x98E, 0x9A7, 0x9C0, 0x9D8, 0x9F1, 0xA09, 0xA21, 0xA39, 
    0xA51, 0xA69, 0xA81, 0xA99, 0xAB1, 0xAC8, 0xAE0, 0xAF7, 0xB0F, 0xB26, 0xB3D, 0xB54, 
    0xB6A, 0xB81, 0xB98, 0xBAE, 0xBC4, 0xBDA, 0xBF0, 0xC06, 0xC1C, 0xC31, 0xC46, 0xC5C, 
    0xC71, 0xC85, 0xC9A, 0xCAE, 0xCC3, 0xCD7, 0xCEB, 0xCFE, 0xD12, 0xD25, 0xD38, 0xD4B, 
    0xD5E, 0xD71, 0xD83, 0xD95, 0xDA7, 0xDB8, 0xDCA, 0xDDB, 0xDEC, 0xDFD, 0xE0D, 0xE1E, 
    0xE2E, 0xE3E, 0xE4D, 0xE5C, 0xE6C, 0xE7A, 0xE89, 0xE97, 0xEA5, 0xEB3, 0xEC1, 0xECE, 
    0xEDB, 0xEE8, 0xEF5, 0xF01, 0xF0D, 0xF18, 0xF24, 0xF2F, 0xF3A, 0xF45, 0xF4F, 0xF59, 
    0xF63, 0xF6C, 0xF75, 0xF7E, 0xF87, 0xF8F, 0xF97, 0xF9F, 0xFA6, 0xFAD, 0xFB4, 0xFBB, 
    0xFC1, 0xFC7, 0xFCD, 0xFD2, 0xFD7, 0xFDC, 0xFE0, 0xFE4, 0xFE8, 0xFEC, 0xFEF, 0xFF2, 
    0xFF5, 0xFF7, 0xFF9, 0xFFB, 0xFFC, 0xFFD, 0xFFE, 0xFFE, 0xFFF, 0xFFE, 0xFFE, 0xFFD, 
    0xFFC, 0xFFB, 0xFF9, 0xFF7, 0xFF5, 0xFF2, 0xFEF, 0xFEC, 0xFE8, 0xFE4, 0xFE0, 0xFDC, 
    0xFD7, 0xFD2, 0xFCD, 0xFC7, 0xFC1, 0xFBB, 0xFB4, 0xFAD, 0xFA6, 0xF9F, 0xF97, 0xF8F, 
    0xF87, 0xF7E, 0xF75, 0xF6C, 0xF63, 0xF59, 0xF4F, 0xF45, 0xF3A, 0xF2F, 0xF24, 0xF18, 
    0xF0D, 0xF01, 0xEF5, 0xEE8, 0xEDB, 0xECE, 0xEC1, 0xEB3, 0xEA5, 0xE97, 0xE89, 0xE7A, 
    0xE6C, 0xE5C, 0xE4D, 0xE3E, 0xE2E, 0xE1E, 0xE0D, 0xDFD, 0xDEC, 0xDDB, 0xDCA, 0xDB8, 
    0xDA7, 0xD95, 0xD83, 0xD71, 0xD5E, 0xD4B, 0xD38, 0xD25, 0xD12, 0xCFE, 0xCEB, 0xCD7, 
    0xCC3, 0xCAE, 0xC9A, 0xC85, 0xC71, 0xC5C, 0xC46, 0xC31, 0xC1C, 0xC06, 0xBF0, 0xBDA, 
    0xBC4, 0xBAE, 0xB98, 0xB81, 0xB6A, 0xB54, 0xB3D, 0xB26, 0xB0F, 0xAF7, 0xAE0, 0xAC8, 
    0xAB1, 0xA99, 0xA81, 0xA69, 0xA51, 0xA39, 0xA21, 0xA09, 0x9F1, 0x9D8, 0x9C0, 0x9A7, 
    0x98E, 0x976, 0x95D, 0x944, 0x92B, 0x913, 0x8FA, 0x8E1, 0x8C8, 0x8AF, 0x896, 0x87D, 
    0x863, 0x84A, 0x831, 0x818, 0x7FF, 0x7E6, 0x7CD, 0x7B4, 0x79B, 0x781, 0x768, 0x74F, 
    0x736, 0x71D, 0x704, 0x6EB, 0x6D3, 0x6BA, 0x6A1, 0x688, 0x670, 0x657, 0x63E, 0x626, 
    0x60D, 0x5F5, 0x5DD, 0x5C5, 0x5AD, 0x595, 0x57D, 0x565, 0x54D, 0x536, 0x51E, 0x507, 
    0x4EF, 0x4D8, 0x4C1, 0x4AA, 0x494, 0x47D, 0x466, 0x450, 0x43A, 0x424, 0x40E, 0x3F8, 
    0x3E2, 0x3CD, 0x3B8, 0x3A2, 0x38D, 0x379, 0x364, 0x350, 0x33B, 0x327, 0x313, 0x300, 
    0x2EC, 0x2D9, 0x2C6, 0x2B3, 0x2A0, 0x28D, 0x27B, 0x269, 0x257, 0x246, 0x234, 0x223, 
    0x212, 0x201, 0x1F1, 0x1E0, 0x1D0, 0x1C0, 0x1B1, 0x1A2, 0x192, 0x184, 0x175, 0x167, 
    0x159, 0x14B, 0x13D, 0x130, 0x123, 0x116, 0x109, 0xFD, 0xF1, 0xE6, 0xDA, 0xCF, 
    0xC4, 0xB9, 0xAF, 0xA5, 0x9B, 0x92, 0x89, 0x80, 0x77, 0x6F, 0x67, 0x5F, 
    0x58, 0x51, 0x4A, 0x43, 0x3D, 0x37, 0x31, 0x2C, 0x27, 0x22, 0x1E, 0x1A, 
    0x16, 0x12, 0x0F, 0x0C, 0x09, 0x07, 0x05, 0x03, 0x02, 0x01, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x05, 0x07, 0x09, 0x0C, 0x0F, 0x12, 
    0x16, 0x1A, 0x1E, 0x22, 0x27, 0x2C, 0x31, 0x37, 0x3D, 0x43, 0x4A, 0x51, 
    0x58, 0x5F, 0x67, 0x6F, 0x77, 0x80, 0x89, 0x92, 0x9B, 0xA5, 0xAF, 0xB9, 
    0xC4, 0xCF, 0xDA, 0xE6, 0xF1, 0xFD, 0x109, 0x116, 0x123, 0x130, 0x13D, 0x14B, 
    0x159, 0x167, 0x175, 0x184, 0x192, 0x1A2, 0x1B1, 0x1C0, 0x1D0, 0x1E0, 0x1F1, 0x201, 
    0x212, 0x223, 0x234, 0x246, 0x257, 0x269, 0x27B, 0x28D, 0x2A0, 0x2B3, 0x2C6, 0x2D9, 
    0x2EC, 0x300, 0x313, 0x327, 0x33B, 0x350, 0x364, 0x379, 0x38D, 0x3A2, 0x3B8, 0x3CD, 
    0x3E2, 0x3F8, 0x40E, 0x424, 0x43A, 0x450, 0x466, 0x47D, 0x494, 0x4AA, 0x4C1, 0x4D8, 
    0x4EF, 0x507, 0x51E, 0x536, 0x54D, 0x565, 0x57D, 0x595, 0x5AD, 0x5C5, 0x5DD, 0x5F5, 
    0x60D, 0x626, 0x63E, 0x657, 0x670, 0x688, 0x6A1, 0x6BA, 0x6D3, 0x6EB, 0x704, 0x71D, 
    0x736, 0x74F, 0x768, 0x781, 0x79B, 0x7B4, 0x7CD, 0x7E6};
uint16_t square_wave[512] = {
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 
    0xFFF, 0xFFF, 0xFFF, 0xFFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

uint16_t sawtooth_wave[512] = {
    0x00, 0x07, 0x0F, 0x17, 0x1F, 0x27, 0x2F, 0x37, 0x3F, 0x47, 0x4F, 0x57, 
    0x5F, 0x67, 0x6F, 0x77, 0x7F, 0x87, 0x8F, 0x97, 0x9F, 0xA7, 0xAF, 0xB7, 
    0xBF, 0xC7, 0xCF, 0xD7, 0xDF, 0xE7, 0xEF, 0xF7, 0xFF, 0x107, 0x10F, 0x117, 
    0x11F, 0x127, 0x12F, 0x137, 0x13F, 0x147, 0x14F, 0x157, 0x15F, 0x167, 0x16F, 0x177, 
    0x17F, 0x187, 0x18F, 0x197, 0x19F, 0x1A7, 0x1AF, 0x1B7, 0x1BF, 0x1C7, 0x1CF, 0x1D7, 
    0x1DF, 0x1E7, 0x1EF, 0x1F7, 0x1FF, 0x207, 0x20F, 0x217, 0x21F, 0x227, 0x22F, 0x237, 
    0x23F, 0x247, 0x24F, 0x257, 0x25F, 0x267, 0x26F, 0x277, 0x27F, 0x287, 0x28F, 0x297, 
    0x29F, 0x2A7, 0x2AF, 0x2B7, 0x2BF, 0x2C7, 0x2CF, 0x2D7, 0x2DF, 0x2E7, 0x2EF, 0x2F7, 
    0x2FF, 0x307, 0x30F, 0x317, 0x31F, 0x327, 0x32F, 0x337, 0x33F, 0x347, 0x34F, 0x357, 
    0x35F, 0x367, 0x36F, 0x377, 0x37F, 0x387, 0x38F, 0x397, 0x39F, 0x3A7, 0x3AF, 0x3B7, 
    0x3BF, 0x3C7, 0x3CF, 0x3D7, 0x3DF, 0x3E7, 0x3EF, 0x3F7, 0x3FF, 0x407, 0x40F, 0x417, 
    0x41F, 0x427, 0x42F, 0x437, 0x43F, 0x447, 0x44F, 0x457, 0x45F, 0x467, 0x46F, 0x477, 
    0x47F, 0x487, 0x48F, 0x497, 0x49F, 0x4A7, 0x4AF, 0x4B7, 0x4BF, 0x4C7, 0x4CF, 0x4D7, 
    0x4DF, 0x4E7, 0x4EF, 0x4F7, 0x4FF, 0x507, 0x50F, 0x517, 0x51F, 0x527, 0x52F, 0x537, 
    0x53F, 0x547, 0x54F, 0x557, 0x55F, 0x567, 0x56F, 0x577, 0x57F, 0x587, 0x58F, 0x597, 
    0x59F, 0x5A7, 0x5AF, 0x5B7, 0x5BF, 0x5C7, 0x5CF, 0x5D7, 0x5DF, 0x5E7, 0x5EF, 0x5F7, 
    0x5FF, 0x607, 0x60F, 0x617, 0x61F, 0x627, 0x62F, 0x637, 0x63F, 0x647, 0x64F, 0x657, 
    0x65F, 0x667, 0x66F, 0x677, 0x67F, 0x687, 0x68F, 0x697, 0x69F, 0x6A7, 0x6AF, 0x6B7, 
    0x6BF, 0x6C7, 0x6CF, 0x6D7, 0x6DF, 0x6E7, 0x6EF, 0x6F7, 0x6FF, 0x707, 0x70F, 0x717, 
    0x71F, 0x727, 0x72F, 0x737, 0x73F, 0x747, 0x74F, 0x757, 0x75F, 0x767, 0x76F, 0x777, 
    0x77F, 0x787, 0x78F, 0x797, 0x79F, 0x7A7, 0x7AF, 0x7B7, 0x7BF, 0x7C7, 0x7CF, 0x7D7, 
    0x7DF, 0x7E7, 0x7EF, 0x7F7, 0x7FF, 0x807, 0x80F, 0x817, 0x81F, 0x827, 0x82F, 0x837, 
    0x83F, 0x847, 0x84F, 0x857, 0x85F, 0x867, 0x86F, 0x877, 0x87F, 0x887, 0x88F, 0x897, 
    0x89F, 0x8A7, 0x8AF, 0x8B7, 0x8BF, 0x8C7, 0x8CF, 0x8D7, 0x8DF, 0x8E7, 0x8EF, 0x8F7, 
    0x8FF, 0x907, 0x90F, 0x917, 0x91F, 0x927, 0x92F, 0x937, 0x93F, 0x947, 0x94F, 0x957, 
    0x95F, 0x967, 0x96F, 0x977, 0x97F, 0x987, 0x98F, 0x997, 0x99F, 0x9A7, 0x9AF, 0x9B7, 
    0x9BF, 0x9C7, 0x9CF, 0x9D7, 0x9DF, 0x9E7, 0x9EF, 0x9F7, 0x9FF, 0xA07, 0xA0F, 0xA17, 
    0xA1F, 0xA27, 0xA2F, 0xA37, 0xA3F, 0xA47, 0xA4F, 0xA57, 0xA5F, 0xA67, 0xA6F, 0xA77, 
    0xA7F, 0xA87, 0xA8F, 0xA97, 0xA9F, 0xAA7, 0xAAF, 0xAB7, 0xABF, 0xAC7, 0xACF, 0xAD7, 
    0xADF, 0xAE7, 0xAEF, 0xAF7, 0xAFF, 0xB07, 0xB0F, 0xB17, 0xB1F, 0xB27, 0xB2F, 0xB37, 
    0xB3F, 0xB47, 0xB4F, 0xB57, 0xB5F, 0xB67, 0xB6F, 0xB77, 0xB7F, 0xB87, 0xB8F, 0xB97, 
    0xB9F, 0xBA7, 0xBAF, 0xBB7, 0xBBF, 0xBC7, 0xBCF, 0xBD7, 0xBDF, 0xBE7, 0xBEF, 0xBF7, 
    0xBFF, 0xC07, 0xC0F, 0xC17, 0xC1F, 0xC27, 0xC2F, 0xC37, 0xC3F, 0xC47, 0xC4F, 0xC57, 
    0xC5F, 0xC67, 0xC6F, 0xC77, 0xC7F, 0xC87, 0xC8F, 0xC97, 0xC9F, 0xCA7, 0xCAF, 0xCB7, 
    0xCBF, 0xCC7, 0xCCF, 0xCD7, 0xCDF, 0xCE7, 0xCEF, 0xCF7, 0xCFF, 0xD07, 0xD0F, 0xD17, 
    0xD1F, 0xD27, 0xD2F, 0xD37, 0xD3F, 0xD47, 0xD4F, 0xD57, 0xD5F, 0xD67, 0xD6F, 0xD77, 
    0xD7F, 0xD87, 0xD8F, 0xD97, 0xD9F, 0xDA7, 0xDAF, 0xDB7, 0xDBF, 0xDC7, 0xDCF, 0xDD7, 
    0xDDF, 0xDE7, 0xDEF, 0xDF7, 0xDFF, 0xE07, 0xE0F, 0xE17, 0xE1F, 0xE27, 0xE2F, 0xE37, 
    0xE3F, 0xE47, 0xE4F, 0xE57, 0xE5F, 0xE67, 0xE6F, 0xE77, 0xE7F, 0xE87, 0xE8F, 0xE97, 
    0xE9F, 0xEA7, 0xEAF, 0xEB7, 0xEBF, 0xEC7, 0xECF, 0xED7, 0xEDF, 0xEE7, 0xEEF, 0xEF7, 
    0xEFF, 0xF07, 0xF0F, 0xF17, 0xF1F, 0xF27, 0xF2F, 0xF37, 0xF3F, 0xF47, 0xF4F, 0xF57, 
    0xF5F, 0xF67, 0xF6F, 0xF77, 0xF7F, 0xF87, 0xF8F, 0xF97, 0xF9F, 0xFA7, 0xFAF, 0xFB7, 
    0xFBF, 0xFC7, 0xFCF, 0xFD7, 0xFDF, 0xFE7, 0xFEF, 0xFF7};

uint16_t organ_pipe_wave[512] = {
    0x7FF, 0x826, 0x84E, 0x875, 0x89D, 0x8C4, 0x8EB, 0x912, 0x939, 0x95F, 0x986, 0x9AC, 
    0x9D2, 0x9F7, 0xA1C, 0xA41, 0xA66, 0xA8A, 0xAAD, 0xAD1, 0xAF3, 0xB16, 0xB38, 0xB59, 
    0xB7A, 0xB9A, 0xBBA, 0xBD9, 0xBF7, 0xC15, 0xC32, 0xC4F, 0xC6B, 0xC86, 0xCA0, 0xCBA, 
    0xCD3, 0xCEC, 0xD03, 0xD1A, 0xD30, 0xD45, 0xD5A, 0xD6E, 0xD81, 0xD93, 0xDA4, 0xDB5, 
    0xDC4, 0xDD3, 0xDE1, 0xDEF, 0xDFB, 0xE07, 0xE12, 0xE1C, 0xE25, 0xE2D, 0xE35, 0xE3C, 
    0xE42, 0xE47, 0xE4C, 0xE4F, 0xE52, 0xE54, 0xE56, 0xE56, 0xE56, 0xE56, 0xE54, 0xE52, 
    0xE4F, 0xE4C, 0xE47, 0xE43, 0xE3D, 0xE37, 0xE30, 0xE29, 0xE21, 0xE19, 0xE10, 0xE07, 
    0xDFD, 0xDF2, 0xDE8, 0xDDC, 0xDD1, 0xDC4, 0xDB8, 0xDAB, 0xD9E, 0xD90, 0xD82, 0xD74, 
    0xD66, 0xD57, 0xD48, 0xD39, 0xD29, 0xD1A, 0xD0A, 0xCFA, 0xCEA, 0xCDA, 0xCCA, 0xCB9, 
    0xCA9, 0xC99, 0xC88, 0xC78, 0xC67, 0xC57, 0xC46, 0xC36, 0xC26, 0xC16, 0xC05, 0xBF5, 
    0xBE6, 0xBD6, 0xBC6, 0xBB7, 0xBA8, 0xB99, 0xB8A, 0xB7B, 0xB6D, 0xB5E, 0xB50, 0xB42, 
    0xB35, 0xB28, 0xB1B, 0xB0E, 0xB01, 0xAF5, 0xAE9, 0xADD, 0xAD2, 0xAC7, 0xABC, 0xAB2, 
    0xAA7, 0xA9E, 0xA94, 0xA8B, 0xA81, 0xA79, 0xA70, 0xA68, 0xA60, 0xA58, 0xA51, 0xA4A, 
    0xA43, 0xA3D, 0xA36, 0xA30, 0xA2A, 0xA25, 0xA1F, 0xA1A, 0xA15, 0xA11, 0xA0C, 0xA08, 
    0xA04, 0xA00, 0x9FC, 0x9F8, 0x9F5, 0x9F2, 0x9EE, 0x9EB, 0x9E8, 0x9E6, 0x9E3, 0x9E0, 
    0x9DD, 0x9DB, 0x9D8, 0x9D6, 0x9D4, 0x9D1, 0x9CF, 0x9CC, 0x9CA, 0x9C8, 0x9C5, 0x9C3, 
    0x9C0, 0x9BE, 0x9BB, 0x9B8, 0x9B5, 0x9B3, 0x9B0, 0x9AD, 0x9A9, 0x9A6, 0x9A3, 0x99F, 
    0x99C, 0x998, 0x994, 0x990, 0x98B, 0x987, 0x982, 0x97E, 0x979, 0x974, 0x96E, 0x969, 
    0x963, 0x95D, 0x957, 0x951, 0x94B, 0x944, 0x93D, 0x936, 0x92F, 0x928, 0x920, 0x919, 
    0x911, 0x909, 0x901, 0x8F8, 0x8F0, 0x8E7, 0x8DE, 0x8D5, 0x8CC, 0x8C3, 0x8B9, 0x8B0, 
    0x8A6, 0x89C, 0x892, 0x888, 0x87E, 0x874, 0x869, 0x85F, 0x854, 0x84A, 0x83F, 0x835, 
    0x82A, 0x81F, 0x815, 0x80A, 0x7FF, 0x7F4, 0x7E9, 0x7DF, 0x7D4, 0x7C9, 0x7BF, 0x7B4, 
    0x7AA, 0x79F, 0x795, 0x78A, 0x780, 0x776, 0x76C, 0x762, 0x758, 0x74E, 0x745, 0x73B, 
    0x732, 0x729, 0x720, 0x717, 0x70E, 0x706, 0x6FD, 0x6F5, 0x6ED, 0x6E5, 0x6DE, 0x6D6, 
    0x6CF, 0x6C8, 0x6C1, 0x6BA, 0x6B3, 0x6AD, 0x6A7, 0x6A1, 0x69B, 0x695, 0x690, 0x68A, 
    0x685, 0x680, 0x67C, 0x677, 0x673, 0x66E, 0x66A, 0x666, 0x662, 0x65F, 0x65B, 0x658, 
    0x655, 0x651, 0x64E, 0x64B, 0x649, 0x646, 0x643, 0x640, 0x63E, 0x63B, 0x639, 0x636, 
    0x634, 0x632, 0x62F, 0x62D, 0x62A, 0x628, 0x626, 0x623, 0x621, 0x61E, 0x61B, 0x618, 
    0x616, 0x613, 0x610, 0x60C, 0x609, 0x606, 0x602, 0x5FE, 0x5FA, 0x5F6, 0x5F2, 0x5ED, 
    0x5E9, 0x5E4, 0x5DF, 0x5D9, 0x5D4, 0x5CE, 0x5C8, 0x5C1, 0x5BB, 0x5B4, 0x5AD, 0x5A6, 
    0x59E, 0x596, 0x58E, 0x585, 0x57D, 0x573, 0x56A, 0x560, 0x557, 0x54C, 0x542, 0x537, 
    0x52C, 0x521, 0x515, 0x509, 0x4FD, 0x4F0, 0x4E3, 0x4D6, 0x4C9, 0x4BC, 0x4AE, 0x4A0, 
    0x491, 0x483, 0x474, 0x465, 0x456, 0x447, 0x438, 0x428, 0x418, 0x409, 0x3F9, 0x3E8, 
    0x3D8, 0x3C8, 0x3B8, 0x3A7, 0x397, 0x386, 0x376, 0x365, 0x355, 0x345, 0x334, 0x324, 
    0x314, 0x304, 0x2F4, 0x2E4, 0x2D5, 0x2C5, 0x2B6, 0x2A7, 0x298, 0x28A, 0x27C, 0x26E, 
    0x260, 0x253, 0x246, 0x23A, 0x22D, 0x222, 0x216, 0x20C, 0x201, 0x1F7, 0x1EE, 0x1E5, 
    0x1DD, 0x1D5, 0x1CE, 0x1C7, 0x1C1, 0x1BB, 0x1B7, 0x1B2, 0x1AF, 0x1AC, 0x1AA, 0x1A8, 
    0x1A8, 0x1A8, 0x1A8, 0x1AA, 0x1AC, 0x1AF, 0x1B2, 0x1B7, 0x1BC, 0x1C2, 0x1C9, 0x1D1, 
    0x1D9, 0x1E2, 0x1EC, 0x1F7, 0x203, 0x20F, 0x21D, 0x22B, 0x23A, 0x249, 0x25A, 0x26B, 
    0x27D, 0x290, 0x2A4, 0x2B9, 0x2CE, 0x2E4, 0x2FB, 0x312, 0x32B, 0x344, 0x35E, 0x378, 
    0x393, 0x3AF, 0x3CC, 0x3E9, 0x407, 0x425, 0x444, 0x464, 0x484, 0x4A5, 0x4C6, 0x4E8, 
    0x50B, 0x52D, 0x551, 0x574, 0x598, 0x5BD, 0x5E2, 0x607, 0x62C, 0x652, 0x678, 0x69F, 
    0x6C5, 0x6EC, 0x713, 0x73A, 0x761, 0x789, 0x7B0, 0x7D8};



