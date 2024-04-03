#include "magic.h"

// generate magic number candidate
u64 generate_magic_number() {
    return get_random_u64_number() & get_random_u64_number() & get_random_u64_number();
}

// rook magic numbers
u64 rook_magic_numbers[64] = {
    0x8A80104000800020ull,
    0x140002000100040ull,
    0x2801880A0017001ull,
    0x100081001000420ull,
    0x200020010080420ull,
    0x3001C0002010008ull,
    0x8480008002000100ull,
    0x2080088004402900ull,
    0x800098204000ull,
    0x2024401000200040ull,
    0x100802000801000ull,
    0x120800800801000ull,
    0x208808088000400ull,
    0x2802200800400ull,
    0x2200800100020080ull,
    0x801000060821100ull,
    0x80044006422000ull,
    0x100808020004000ull,
    0x12108A0010204200ull,
    0x140848010000802ull,
    0x481828014002800ull,
    0x8094004002004100ull,
    0x4010040010010802ull,
    0x20008806104ull,
    0x100400080208000ull,
    0x2040002120081000ull,
    0x21200680100081ull,
    0x20100080080080ull,
    0x2000A00200410ull,
    0x20080800400ull,
    0x80088400100102ull,
    0x80004600042881ull,
    0x4040008040800020ull,
    0x440003000200801ull,
    0x4200011004500ull,
    0x188020010100100ull,
    0x14800401802800ull,
    0x2080040080800200ull,
    0x124080204001001ull,
    0x200046502000484ull,
    0x480400080088020ull,
    0x1000422010034000ull,
    0x30200100110040ull,
    0x100021010009ull,
    0x2002080100110004ull,
    0x202008004008002ull,
    0x20020004010100ull,
    0x2048440040820001ull,
    0x101002200408200ull,
    0x40802000401080ull,
    0x4008142004410100ull,
    0x2060820C0120200ull,
    0x1001004080100ull,
    0x20C020080040080ull,
    0x2935610830022400ull,
    0x44440041009200ull,
    0x280001040802101ull,
    0x2100190040002085ull,
    0x80C0084100102001ull,
    0x4024081001000421ull,
    0x20030A0244872ull,
    0x12001008414402ull,
    0x2006104900A0804ull,
    0x1004081002402ull
};

//bishop magic numbers
u64 bishop_magic_numbers[64] = {
    0x40040844404084ull,
    0x2004208A004208ull,
    0x10190041080202ull,
    0x108060845042010ull,
    0x581104180800210ull,
    0x2112080446200010ull,
    0x1080820820060210ull,
    0x3C0808410220200ull,
    0x4050404440404ull,
    0x21001420088ull,
    0x24D0080801082102ull,
    0x1020A0A020400ull,
    0x40308200402ull,
    0x4011002100800ull,
    0x401484104104005ull,
    0x801010402020200ull,
    0x400210C3880100ull,
    0x404022024108200ull,
    0x810018200204102ull,
    0x4002801A02003ull,
    0x85040820080400ull,
    0x810102C808880400ull,
    0xE900410884800ull,
    0x8002020480840102ull,
    0x220200865090201ull,
    0x2010100A02021202ull,
    0x152048408022401ull,
    0x20080002081110ull,
    0x4001001021004000ull,
    0x800040400A011002ull,
    0xE4004081011002ull,
    0x1C004001012080ull,
    0x8004200962A00220ull,
    0x8422100208500202ull,
    0x2000402200300C08ull,
    0x8646020080080080ull,
    0x80020A0200100808ull,
    0x2010004880111000ull,
    0x623000A080011400ull,
    0x42008C0340209202ull,
    0x209188240001000ull,
    0x400408A884001800ull,
    0x110400A6080400ull,
    0x1840060A44020800ull,
    0x90080104000041ull,
    0x201011000808101ull,
    0x1A2208080504F080ull,
    0x8012020600211212ull,
    0x500861011240000ull,
    0x180806108200800ull,
    0x4000020E01040044ull,
    0x300000261044000Aull,
    0x802241102020002ull,
    0x20906061210001ull,
    0x5A84841004010310ull,
    0x4010801011C04ull,
    0xA010109502200ull,
    0x4A02012000ull,
    0x500201010098B028ull,
    0x8040002811040900ull,
    0x28000010020204ull,
    0x6000020202D0240ull,
    0x8918844842082200ull,
    0x4010011029020020ull
};

// find appropriate magic number
u64 find_magic_number(int square, int relevant_bits, int bishop) {

    u64 occupancies[4096];

    // init attack tables
    u64 attacks[4096];

    u64 used_attacks[4096];

    u64 attack_mask = bishop ? mask_bishop_attacks(square) : mask_rook_attacks(square);

    // init occupancy indices
    int occupancy_indices = 1 << relevant_bits;

    // loop over occupancy indices
    for (int index = 0; index < occupancy_indices; index++) {

        //initialize occupancies
        occupancies[index] = set_occupancy(index, relevant_bits, attack_mask);

        attacks[index] = bishop ? bishop_attacks_otf(square, occupancies[index]) :
                                    rook_attacks_otf(square, occupancies[index]);

    }

    // test magic numbers
    for (int count = 0; count < 10000000; count++) {
        // generate magic number
        u64 magic_number = generate_magic_number();

        // skip magic number if invalid
        if (cpopcount64((attack_mask * magic_number) & 0xFF00000000000000) < 6) continue;

        // init used attacks
        for (int i = 0; i < 4096; i++) {
            used_attacks[i] = 0ull;
        }

        // init index & fail flags
        int index, fail;

        // test magic index loop 
        for (index = 0, fail = 0; !fail && index < occupancy_indices; ++index) {

            //init magic index
            int magic_index = (int) ((occupancies[index] * magic_number) >> (64 - relevant_bits));

            // on empty index available
            if (used_attacks[magic_index] == 0ull) {
                //init used attacks
                used_attacks[magic_index] = attacks[index];

            } else if (used_attacks[magic_index] != attacks[index]) {
                fail = 1;
            }
        }

        if (!fail) {
            return magic_number;
        }
    }

    printf("failed");
    return 0ull;
}

// init magic numbers
void init_magic_numbers() {
    //
    for (int square = 0; square < 64; square++) {
        // init rook magic numbers
        printf(" 0x%llXull,\n", find_magic_number(square, rook_relevant_bits[square], rook));
    }

    printf("\n\n");

    for (int square = 0; square < 64; square++) {
        // init bishop magic numbers
        printf(" 0x%llXull,\n", find_magic_number(square, bishop_relevant_bits[square], bishop));
    }
}

void init_sliders_attacks(int bishop) {
    
    for (int square = 0; square < 64; square++) {
        // init bishop and rook masks
        bishop_masks[square] = mask_bishop_attacks(square);
        rook_masks[square] = mask_rook_attacks(square);

        u64 attack_mask = bishop ? bishop_masks[square] : rook_masks[square];

        // init relevant occupancy bit count
        int relevant_bits = bishop ? bishop_relevant_bits[square] : rook_relevant_bits[square];

        // init occupancy indices
        int occupancy_indices = (1 << relevant_bits);

        for (int index = 0; index < occupancy_indices; index++) {

            u64 occupancy = set_occupancy(index, relevant_bits, attack_mask);

            if (bishop) {

                int magic_index = (occupancy * bishop_magic_numbers[square]) >> (64 - relevant_bits);

                bishop_attacks[square][magic_index] = bishop_attacks_otf(square, occupancy);

            } else {
                
                int magic_index = (occupancy * rook_magic_numbers[square]) >> (64 - relevant_bits);

                rook_attacks[square][magic_index] = rook_attacks_otf(square, occupancy);

            }
        }
    }
}

