/*
 * GS1 AI checker generated by "backend/tools/gen_gs1_lint.php" from
 * https://raw.githubusercontent.com/bwipp/postscriptbarcode/master/contrib/development/gs1-format-spec.txt
 */
/*
    libzint - the open source barcode library
    Copyright (C) 2021 Robin Stuart <rstuart114@gmail.com>

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Neither the name of the project nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
 */

#ifndef GS1_LINT_H
#define GS1_LINT_H

/* N18,csum,key */
static int n18_csum_key(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len == 18
            && csum(data, data_len, 0, 18, 18, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && key(data, data_len, 0, 18, 18, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 18, 18, p_err_no, p_err_posn, err_msg)
            && csum(data, data_len, 0, 18, 18, p_err_no, p_err_posn, err_msg, 0)
            && key(data, data_len, 0, 18, 18, p_err_no, p_err_posn, err_msg, 0);
}

/* N14,csum,key */
static int n14_csum_key(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len == 14
            && csum(data, data_len, 0, 14, 14, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && key(data, data_len, 0, 14, 14, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 14, 14, p_err_no, p_err_posn, err_msg)
            && csum(data, data_len, 0, 14, 14, p_err_no, p_err_posn, err_msg, 0)
            && key(data, data_len, 0, 14, 14, p_err_no, p_err_posn, err_msg, 0);
}

/* X..20 */
static int x__20(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 20
            && cset82(data, data_len, 0, 1, 20, p_err_no, p_err_posn, err_msg);
}

/* N6,yymmd0 */
static int n6_yymmd0(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len == 6
            && yymmd0(data, data_len, 0, 6, 6, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 6, 6, p_err_no, p_err_posn, err_msg)
            && yymmd0(data, data_len, 0, 6, 6, p_err_no, p_err_posn, err_msg, 0);
}

/* N2 */
static int n2(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len == 2
            && numeric(data, data_len, 0, 2, 2, p_err_no, p_err_posn, err_msg);
}

/* X..28 */
static int x__28(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 28
            && cset82(data, data_len, 0, 1, 28, p_err_no, p_err_posn, err_msg);
}

/* X..30 */
static int x__30(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 30
            && cset82(data, data_len, 0, 1, 30, p_err_no, p_err_posn, err_msg);
}

/* N..6 */
static int n__6(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 6
            && numeric(data, data_len, 0, 1, 6, p_err_no, p_err_posn, err_msg);
}

/* N13,csum,key X0..17 */
static int n13_csum_key_x0__17(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 13 && data_len <= 30
            && csum(data, data_len, 0, 13, 13, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && key(data, data_len, 0, 13, 13, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 13, 13, p_err_no, p_err_posn, err_msg)
            && csum(data, data_len, 0, 13, 13, p_err_no, p_err_posn, err_msg, 0)
            && key(data, data_len, 0, 13, 13, p_err_no, p_err_posn, err_msg, 0)
            && cset82(data, data_len, 13, 0, 17, p_err_no, p_err_posn, err_msg);
}

/* N13,csum,key N0..12 */
static int n13_csum_key_n0__12(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 13 && data_len <= 25
            && csum(data, data_len, 0, 13, 13, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && key(data, data_len, 0, 13, 13, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 13, 13, p_err_no, p_err_posn, err_msg)
            && csum(data, data_len, 0, 13, 13, p_err_no, p_err_posn, err_msg, 0)
            && key(data, data_len, 0, 13, 13, p_err_no, p_err_posn, err_msg, 0)
            && numeric(data, data_len, 13, 0, 12, p_err_no, p_err_posn, err_msg);
}

/* N..8 */
static int n__8(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 8
            && numeric(data, data_len, 0, 1, 8, p_err_no, p_err_posn, err_msg);
}

/* N6 */
static int n6(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len == 6
            && numeric(data, data_len, 0, 6, 6, p_err_no, p_err_posn, err_msg);
}

/* N..15 */
static int n__15(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 15
            && numeric(data, data_len, 0, 1, 15, p_err_no, p_err_posn, err_msg);
}

/* N3,iso4217 N..15 */
static int n3_iso4217_n__15(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 4 && data_len <= 18
            && iso4217(data, data_len, 0, 3, 3, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 3, 3, p_err_no, p_err_posn, err_msg)
            && iso4217(data, data_len, 0, 3, 3, p_err_no, p_err_posn, err_msg, 0)
            && numeric(data, data_len, 3, 1, 15, p_err_no, p_err_posn, err_msg);
}

/* N4 */
static int n4(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len == 4
            && numeric(data, data_len, 0, 4, 4, p_err_no, p_err_posn, err_msg);
}

/* X..30,key */
static int x__30_key(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 30
            && key(data, data_len, 0, 1, 30, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && cset82(data, data_len, 0, 1, 30, p_err_no, p_err_posn, err_msg)
            && key(data, data_len, 0, 1, 30, p_err_no, p_err_posn, err_msg, 0);
}

/* N17,csum,key */
static int n17_csum_key(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len == 17
            && csum(data, data_len, 0, 17, 17, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && key(data, data_len, 0, 17, 17, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 17, 17, p_err_no, p_err_posn, err_msg)
            && csum(data, data_len, 0, 17, 17, p_err_no, p_err_posn, err_msg, 0)
            && key(data, data_len, 0, 17, 17, p_err_no, p_err_posn, err_msg, 0);
}

/* N13,csum,key */
static int n13_csum_key(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len == 13
            && csum(data, data_len, 0, 13, 13, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && key(data, data_len, 0, 13, 13, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 13, 13, p_err_no, p_err_posn, err_msg)
            && csum(data, data_len, 0, 13, 13, p_err_no, p_err_posn, err_msg, 0)
            && key(data, data_len, 0, 13, 13, p_err_no, p_err_posn, err_msg, 0);
}

/* N3,iso3166 X..9 */
static int n3_iso3166_x__9(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 4 && data_len <= 12
            && iso3166(data, data_len, 0, 3, 3, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 3, 3, p_err_no, p_err_posn, err_msg)
            && iso3166(data, data_len, 0, 3, 3, p_err_no, p_err_posn, err_msg, 0)
            && cset82(data, data_len, 3, 1, 9, p_err_no, p_err_posn, err_msg);
}

/* N3,iso3166 */
static int n3_iso3166(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len == 3
            && iso3166(data, data_len, 0, 3, 3, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 3, 3, p_err_no, p_err_posn, err_msg)
            && iso3166(data, data_len, 0, 3, 3, p_err_no, p_err_posn, err_msg, 0);
}

/* N..15,iso3166list */
static int n__15_iso3166list(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 15
            && iso3166list(data, data_len, 0, 1, 15, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 1, 15, p_err_no, p_err_posn, err_msg)
            && iso3166list(data, data_len, 0, 1, 15, p_err_no, p_err_posn, err_msg, 0);
}

/* X..3 */
static int x__3(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 3
            && cset82(data, data_len, 0, 1, 3, p_err_no, p_err_posn, err_msg);
}

/* X..35,pcenc */
static int x__35_pcenc(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 35
            && pcenc(data, data_len, 0, 1, 35, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && cset82(data, data_len, 0, 1, 35, p_err_no, p_err_posn, err_msg)
            && pcenc(data, data_len, 0, 1, 35, p_err_no, p_err_posn, err_msg, 0);
}

/* X..70,pcenc */
static int x__70_pcenc(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 70
            && pcenc(data, data_len, 0, 1, 70, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && cset82(data, data_len, 0, 1, 70, p_err_no, p_err_posn, err_msg)
            && pcenc(data, data_len, 0, 1, 70, p_err_no, p_err_posn, err_msg, 0);
}

/* X2,iso3166alpha2 */
static int x2_iso3166alpha2(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len == 2
            && iso3166alpha2(data, data_len, 0, 2, 2, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && cset82(data, data_len, 0, 2, 2, p_err_no, p_err_posn, err_msg)
            && iso3166alpha2(data, data_len, 0, 2, 2, p_err_no, p_err_posn, err_msg, 0);
}

/* N1,yesno */
static int n1_yesno(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len == 1
            && yesno(data, data_len, 0, 1, 1, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 1, 1, p_err_no, p_err_posn, err_msg)
            && yesno(data, data_len, 0, 1, 1, p_err_no, p_err_posn, err_msg, 0);
}

/* N6,yymmd0 N4,hhmm */
static int n6_yymmd0_n4_hhmm(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len == 10
            && yymmd0(data, data_len, 0, 6, 6, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && hhmm(data, data_len, 6, 4, 4, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 6, 6, p_err_no, p_err_posn, err_msg)
            && yymmd0(data, data_len, 0, 6, 6, p_err_no, p_err_posn, err_msg, 0)
            && numeric(data, data_len, 6, 4, 4, p_err_no, p_err_posn, err_msg)
            && hhmm(data, data_len, 6, 4, 4, p_err_no, p_err_posn, err_msg, 0);
}

/* N6,yymmdd */
static int n6_yymmdd(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len == 6
            && yymmdd(data, data_len, 0, 6, 6, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 6, 6, p_err_no, p_err_posn, err_msg)
            && yymmdd(data, data_len, 0, 6, 6, p_err_no, p_err_posn, err_msg, 0);
}

/* N13 */
static int n13(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len == 13
            && numeric(data, data_len, 0, 13, 13, p_err_no, p_err_posn, err_msg);
}

/* N6,yymmdd N4,hhmm */
static int n6_yymmdd_n4_hhmm(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len == 10
            && yymmdd(data, data_len, 0, 6, 6, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && hhmm(data, data_len, 6, 4, 4, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 6, 6, p_err_no, p_err_posn, err_msg)
            && yymmdd(data, data_len, 0, 6, 6, p_err_no, p_err_posn, err_msg, 0)
            && numeric(data, data_len, 6, 4, 4, p_err_no, p_err_posn, err_msg)
            && hhmm(data, data_len, 6, 4, 4, p_err_no, p_err_posn, err_msg, 0);
}

/* N..4 */
static int n__4(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 4
            && numeric(data, data_len, 0, 1, 4, p_err_no, p_err_posn, err_msg);
}

/* X..12 */
static int x__12(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 12
            && cset82(data, data_len, 0, 1, 12, p_err_no, p_err_posn, err_msg);
}

/* N6,yymmdd N0..6,yymmdd */
static int n6_yymmdd_n0__6_yymmdd(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 6 && data_len <= 12
            && yymmdd(data, data_len, 0, 6, 6, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && yymmdd(data, data_len, 6, 0, 6, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 6, 6, p_err_no, p_err_posn, err_msg)
            && yymmdd(data, data_len, 0, 6, 6, p_err_no, p_err_posn, err_msg, 0)
            && numeric(data, data_len, 6, 0, 6, p_err_no, p_err_posn, err_msg)
            && yymmdd(data, data_len, 6, 0, 6, p_err_no, p_err_posn, err_msg, 0);
}

/* X..10 */
static int x__10(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 10
            && cset82(data, data_len, 0, 1, 10, p_err_no, p_err_posn, err_msg);
}

/* X..2 */
static int x__2(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 2
            && cset82(data, data_len, 0, 1, 2, p_err_no, p_err_posn, err_msg);
}

/* N3,iso3166999 X..27 */
static int n3_iso3166999_x__27(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 4 && data_len <= 30
            && iso3166999(data, data_len, 0, 3, 3, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 3, 3, p_err_no, p_err_posn, err_msg)
            && iso3166999(data, data_len, 0, 3, 3, p_err_no, p_err_posn, err_msg, 0)
            && cset82(data, data_len, 3, 1, 27, p_err_no, p_err_posn, err_msg);
}

/* N1 X1 X1 X1,importeridx */
static int n1_x1_x1_x1_importeridx(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len == 4
            && importeridx(data, data_len, 3, 1, 1, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 1, 1, p_err_no, p_err_posn, err_msg)
            && cset82(data, data_len, 1, 1, 1, p_err_no, p_err_posn, err_msg)
            && cset82(data, data_len, 2, 1, 1, p_err_no, p_err_posn, err_msg)
            && cset82(data, data_len, 3, 1, 1, p_err_no, p_err_posn, err_msg)
            && importeridx(data, data_len, 3, 1, 1, p_err_no, p_err_posn, err_msg, 0);
}

/* X2 X..28 */
static int x2_x__28(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 3 && data_len <= 30
            && cset82(data, data_len, 0, 2, 2, p_err_no, p_err_posn, err_msg)
            && cset82(data, data_len, 2, 1, 28, p_err_no, p_err_posn, err_msg);
}

/* N4,nonzero N5,nonzero N3,nonzero N1,winding N1 */
static int n4_nonzero_n5_nonzero_n3_nonzero_n1_winding_n1(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len == 14
            && nonzero(data, data_len, 0, 4, 4, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && nonzero(data, data_len, 4, 5, 5, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && nonzero(data, data_len, 9, 3, 3, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && winding(data, data_len, 12, 1, 1, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 4, 4, p_err_no, p_err_posn, err_msg)
            && nonzero(data, data_len, 0, 4, 4, p_err_no, p_err_posn, err_msg, 0)
            && numeric(data, data_len, 4, 5, 5, p_err_no, p_err_posn, err_msg)
            && nonzero(data, data_len, 4, 5, 5, p_err_no, p_err_posn, err_msg, 0)
            && numeric(data, data_len, 9, 3, 3, p_err_no, p_err_posn, err_msg)
            && nonzero(data, data_len, 9, 3, 3, p_err_no, p_err_posn, err_msg, 0)
            && numeric(data, data_len, 12, 1, 1, p_err_no, p_err_posn, err_msg)
            && winding(data, data_len, 12, 1, 1, p_err_no, p_err_posn, err_msg, 0)
            && numeric(data, data_len, 13, 1, 1, p_err_no, p_err_posn, err_msg);
}

/* N1,zero N13,csum,key X0..16 */
static int n1_zero_n13_csum_key_x0__16(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 14 && data_len <= 30
            && zero(data, data_len, 0, 1, 1, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && csum(data, data_len, 1, 13, 13, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && key(data, data_len, 1, 13, 13, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 1, 1, p_err_no, p_err_posn, err_msg)
            && zero(data, data_len, 0, 1, 1, p_err_no, p_err_posn, err_msg, 0)
            && numeric(data, data_len, 1, 13, 13, p_err_no, p_err_posn, err_msg)
            && csum(data, data_len, 1, 13, 13, p_err_no, p_err_posn, err_msg, 0)
            && key(data, data_len, 1, 13, 13, p_err_no, p_err_posn, err_msg, 0)
            && cset82(data, data_len, 14, 0, 16, p_err_no, p_err_posn, err_msg);
}

/* N14,csum N4,pieceoftotal */
static int n14_csum_n4_pieceoftotal(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len == 18
            && csum(data, data_len, 0, 14, 14, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && pieceoftotal(data, data_len, 14, 4, 4, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 14, 14, p_err_no, p_err_posn, err_msg)
            && csum(data, data_len, 0, 14, 14, p_err_no, p_err_posn, err_msg, 0)
            && numeric(data, data_len, 14, 4, 4, p_err_no, p_err_posn, err_msg)
            && pieceoftotal(data, data_len, 14, 4, 4, p_err_no, p_err_posn, err_msg, 0);
}

/* X..34,iban */
static int x__34_iban(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 34
            && iban(data, data_len, 0, 1, 34, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && cset82(data, data_len, 0, 1, 34, p_err_no, p_err_posn, err_msg)
            && iban(data, data_len, 0, 1, 34, p_err_no, p_err_posn, err_msg, 0);
}

/* N8,yymmddhh N0..4,mmoptss */
static int n8_yymmddhh_n0__4_mmoptss(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 8 && data_len <= 12
            && yymmddhh(data, data_len, 0, 8, 8, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && mmoptss(data, data_len, 8, 0, 4, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 8, 8, p_err_no, p_err_posn, err_msg)
            && yymmddhh(data, data_len, 0, 8, 8, p_err_no, p_err_posn, err_msg, 0)
            && numeric(data, data_len, 8, 0, 4, p_err_no, p_err_posn, err_msg)
            && mmoptss(data, data_len, 8, 0, 4, p_err_no, p_err_posn, err_msg, 0);
}

/* X..50 */
static int x__50(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 50
            && cset82(data, data_len, 0, 1, 50, p_err_no, p_err_posn, err_msg);
}

/* C..30,key */
static int c__30_key(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 30
            && key(data, data_len, 0, 1, 30, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && cset39(data, data_len, 0, 1, 30, p_err_no, p_err_posn, err_msg)
            && key(data, data_len, 0, 1, 30, p_err_no, p_err_posn, err_msg, 0);
}

/* N..12,nozeroprefix */
static int n__12_nozeroprefix(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 12
            && nozeroprefix(data, data_len, 0, 1, 12, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 1, 12, p_err_no, p_err_posn, err_msg)
            && nozeroprefix(data, data_len, 0, 1, 12, p_err_no, p_err_posn, err_msg, 0);
}

/* N18,csum */
static int n18_csum(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len == 18
            && csum(data, data_len, 0, 18, 18, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && numeric(data, data_len, 0, 18, 18, p_err_no, p_err_posn, err_msg)
            && csum(data, data_len, 0, 18, 18, p_err_no, p_err_posn, err_msg, 0);
}

/* N..10 */
static int n__10(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 10
            && numeric(data, data_len, 0, 1, 10, p_err_no, p_err_posn, err_msg);
}

/* X..25 */
static int x__25(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 25
            && cset82(data, data_len, 0, 1, 25, p_err_no, p_err_posn, err_msg);
}

/* X..70,couponcode */
static int x__70_couponcode(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 70
            && couponcode(data, data_len, 0, 1, 70, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && cset82(data, data_len, 0, 1, 70, p_err_no, p_err_posn, err_msg)
            && couponcode(data, data_len, 0, 1, 70, p_err_no, p_err_posn, err_msg, 0);
}

/* X..70,couponposoffer */
static int x__70_couponposoffer(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 70
            && couponposoffer(data, data_len, 0, 1, 70, p_err_no, p_err_posn, err_msg, 1 /*length_only*/)
            && cset82(data, data_len, 0, 1, 70, p_err_no, p_err_posn, err_msg)
            && couponposoffer(data, data_len, 0, 1, 70, p_err_no, p_err_posn, err_msg, 0);
}

/* X..70 */
static int x__70(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 70
            && cset82(data, data_len, 0, 1, 70, p_err_no, p_err_posn, err_msg);
}

/* X..90 */
static int x__90(const unsigned char *data, const int data_len,
            int *p_err_no, int *p_err_posn, char err_msg[50]) {
    return data_len >= 1 && data_len <= 90
            && cset82(data, data_len, 0, 1, 90, p_err_no, p_err_posn, err_msg);
}

/* Entry point. Returns 1 on success, 0 on failure: `*p_err_no` set to 1 if unknown AI, 2 if bad data length */
static int gs1_lint(const int ai, const unsigned char *data, const int data_len, int *p_err_no, int *p_err_posn,
            char err_msg[50]) {

    /* Assume data length failure */
    *p_err_no = 2;

    if (ai < 100) {

        if (ai == 0) {
            return n18_csum_key(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 1 || ai == 2) {
            return n14_csum_key(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 10 || ai == 21 || ai == 22) {
            return x__20(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai >= 11 && ai <= 17) {
            return n6_yymmd0(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 20) {
            return n2(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 30 || ai == 37) {
            return n__8(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 90) {
            return x__30(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai >= 91) {
            return x__90(data, data_len, p_err_no, p_err_posn, err_msg);
        }

    } else if (ai < 300) {

        if (ai == 235) {
            return x__28(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 240 || ai == 241 || ai == 250 || ai == 251) {
            return x__30(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 242) {
            return n__6(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 243 || ai == 254) {
            return x__20(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 253) {
            return n13_csum_key_x0__17(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 255) {
            return n13_csum_key_n0__12(data, data_len, p_err_no, p_err_posn, err_msg);
        }

    } else if (ai < 500) {

        if (ai == 400 || ai == 403) {
            return x__30(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 401) {
            return x__30_key(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 402) {
            return n17_csum_key(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai >= 410 && ai <= 417) {
            return n13_csum_key(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 420) {
            return x__20(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 421) {
            return n3_iso3166_x__9(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 422 || ai == 424 || ai == 426) {
            return n3_iso3166(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 423 || ai == 425) {
            return n__15_iso3166list(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 427) {
            return x__3(data, data_len, p_err_no, p_err_posn, err_msg);
        }

    } else if (ai < 800) {

        if (ai >= 710 && ai <= 714) {
            return x__20(data, data_len, p_err_no, p_err_posn, err_msg);
        }

    } else if (ai < 3200) {

        if ((ai >= 3100 && ai <= 3105) || (ai >= 3110 && ai <= 3115) || (ai >= 3120 && ai <= 3125)
                || (ai >= 3130 && ai <= 3135) || (ai >= 3140 && ai <= 3145) || (ai >= 3150 && ai <= 3155)
                || (ai >= 3160 && ai <= 3165)) {
            return n6(data, data_len, p_err_no, p_err_posn, err_msg);
        }

    } else if (ai < 3300) {

        if (ai <= 3205 || (ai >= 3210 && ai <= 3215) || (ai >= 3220 && ai <= 3225) || (ai >= 3230 && ai <= 3235)
                || (ai >= 3240 && ai <= 3245) || (ai >= 3250 && ai <= 3255) || (ai >= 3260 && ai <= 3265)
                || (ai >= 3270 && ai <= 3275) || (ai >= 3280 && ai <= 3285) || (ai >= 3290 && ai <= 3295)) {
            return n6(data, data_len, p_err_no, p_err_posn, err_msg);
        }

    } else if (ai < 3400) {

        if (ai <= 3305 || (ai >= 3310 && ai <= 3315) || (ai >= 3320 && ai <= 3325) || (ai >= 3330 && ai <= 3335)
                || (ai >= 3340 && ai <= 3345) || (ai >= 3350 && ai <= 3355) || (ai >= 3360 && ai <= 3365)
                || (ai >= 3370 && ai <= 3375)) {
            return n6(data, data_len, p_err_no, p_err_posn, err_msg);
        }

    } else if (ai < 3500) {

        if (ai <= 3405 || (ai >= 3410 && ai <= 3415) || (ai >= 3420 && ai <= 3425) || (ai >= 3430 && ai <= 3435)
                || (ai >= 3440 && ai <= 3445) || (ai >= 3450 && ai <= 3455) || (ai >= 3460 && ai <= 3465)
                || (ai >= 3470 && ai <= 3475) || (ai >= 3480 && ai <= 3485) || (ai >= 3490 && ai <= 3495)) {
            return n6(data, data_len, p_err_no, p_err_posn, err_msg);
        }

    } else if (ai < 3600) {

        if (ai <= 3505 || (ai >= 3510 && ai <= 3515) || (ai >= 3520 && ai <= 3525) || (ai >= 3530 && ai <= 3535)
                || (ai >= 3540 && ai <= 3545) || (ai >= 3550 && ai <= 3555) || (ai >= 3560 && ai <= 3565)
                || (ai >= 3570 && ai <= 3575)) {
            return n6(data, data_len, p_err_no, p_err_posn, err_msg);
        }

    } else if (ai < 3700) {

        if (ai <= 3605 || (ai >= 3610 && ai <= 3615) || (ai >= 3620 && ai <= 3625) || (ai >= 3630 && ai <= 3635)
                || (ai >= 3640 && ai <= 3645) || (ai >= 3650 && ai <= 3655) || (ai >= 3660 && ai <= 3665)
                || (ai >= 3670 && ai <= 3675) || (ai >= 3680 && ai <= 3685) || (ai >= 3690 && ai <= 3695)) {
            return n6(data, data_len, p_err_no, p_err_posn, err_msg);
        }

    } else if (ai < 4000) {

        if ((ai >= 3900 && ai <= 3909) || (ai >= 3920 && ai <= 3929)) {
            return n__15(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if ((ai >= 3910 && ai <= 3919) || (ai >= 3930 && ai <= 3939)) {
            return n3_iso4217_n__15(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai >= 3940 && ai <= 3943) {
            return n4(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai >= 3950 && ai <= 3955) {
            return n6(data, data_len, p_err_no, p_err_posn, err_msg);
        }

    } else if (ai < 4400) {

        if (ai == 4300 || ai == 4301 || ai == 4310 || ai == 4311 || ai == 4320) {
            return x__35_pcenc(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if ((ai >= 4302 && ai <= 4306) || (ai >= 4312 && ai <= 4316)) {
            return x__70_pcenc(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 4307 || ai == 4317) {
            return x2_iso3166alpha2(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 4308 || ai == 4319) {
            return x__30(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 4318) {
            return x__20(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai >= 4321 && ai <= 4323) {
            return n1_yesno(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 4324 || ai == 4325) {
            return n6_yymmd0_n4_hhmm(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 4326) {
            return n6_yymmdd(data, data_len, p_err_no, p_err_posn, err_msg);
        }

    } else if (ai < 7100) {

        if (ai == 7001) {
            return n13(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 7002) {
            return x__30(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 7003) {
            return n6_yymmdd_n4_hhmm(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 7004) {
            return n__4(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 7005) {
            return x__12(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 7006) {
            return n6_yymmdd(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 7007) {
            return n6_yymmdd_n0__6_yymmdd(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 7008) {
            return x__3(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 7009) {
            return x__10(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 7010) {
            return x__2(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai >= 7020 && ai <= 7022) {
            return x__20(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 7023) {
            return x__30_key(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai >= 7030 && ai <= 7039) {
            return n3_iso3166999_x__27(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 7040) {
            return n1_x1_x1_x1_importeridx(data, data_len, p_err_no, p_err_posn, err_msg);
        }

    } else if (ai < 7300) {

        if (ai >= 7230 && ai <= 7239) {
            return x2_x__28(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 7240) {
            return x__20(data, data_len, p_err_no, p_err_posn, err_msg);
        }

    } else if (ai < 8100) {

        if (ai == 8001) {
            return n4_nonzero_n5_nonzero_n3_nonzero_n1_winding_n1(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 8002 || ai == 8012) {
            return x__20(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 8003) {
            return n1_zero_n13_csum_key_x0__16(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 8004 || ai == 8013) {
            return x__30_key(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 8005) {
            return n6(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 8006 || ai == 8026) {
            return n14_csum_n4_pieceoftotal(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 8007) {
            return x__34_iban(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 8008) {
            return n8_yymmddhh_n0__4_mmoptss(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 8009) {
            return x__50(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 8010) {
            return c__30_key(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 8011) {
            return n__12_nozeroprefix(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 8017 || ai == 8018) {
            return n18_csum(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 8019) {
            return n__10(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 8020) {
            return x__25(data, data_len, p_err_no, p_err_posn, err_msg);
        }

    } else if (ai < 8200) {

        if (ai == 8110) {
            return x__70_couponcode(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 8111) {
            return n4(data, data_len, p_err_no, p_err_posn, err_msg);
        }
        if (ai == 8112) {
            return x__70_couponposoffer(data, data_len, p_err_no, p_err_posn, err_msg);
        }

    } else if (ai < 8300) {

        if (ai == 8200) {
            return x__70(data, data_len, p_err_no, p_err_posn, err_msg);
        }
    }

    /* Unknown AI */
    *p_err_no = 1;
    return 0;
}

#endif /* GS1_LINT_H */
