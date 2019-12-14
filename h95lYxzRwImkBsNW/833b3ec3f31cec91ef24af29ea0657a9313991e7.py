""" Python Character Mapping Codec iso8859_6 generated from 'MAPPINGS/ISO8859/8859-6.TXT' with gencodec.py.

"""#"

import codecs

### Codec APIs

class Codec(codecs.Codec):

    def encode(self,input,errors='strict'):
        return codecs.charmap_encode(input,errors,encoding_table)

    def decode(self,input,errors='strict'):
        return codecs.charmap_decode(input,errors,decoding_table)

class IncrementalEncoder(codecs.IncrementalEncoder):
    def encode(self, input, final=False):
        return codecs.charmap_encode(input,self.errors,encoding_table)[0]

class IncrementalDecoder(codecs.IncrementalDecoder):
    def decode(self, input, final=False):
        return codecs.charmap_decode(input,self.errors,decoding_table)[0]

class StreamWriter(Codec,codecs.StreamWriter):
    pass

class StreamReader(Codec,codecs.StreamReader):
    pass

### encodings module API

def getregentry():
    return codecs.CodecInfo(
        name='iso8859-6',
        encode=Codec().encode,
        decode=Codec().decode,
        incrementalencoder=IncrementalEncoder,
        incrementaldecoder=IncrementalDecoder,
        streamreader=StreamReader,
        streamwriter=StreamWriter,
    )


### Decoding Table

decoding_table = (
    u'\x00'     #  0x00 -> NULL
    u'\x01'     #  0x01 -> START OF HEADING
    u'\x02'     #  0x02 -> START OF TEXT
    u'\x03'     #  0x03 -> END OF TEXT
    u'\x04'     #  0x04 -> END OF TRANSMISSION
    u'\x05'     #  0x05 -> ENQUIRY
    u'\x06'     #  0x06 -> ACKNOWLEDGE
    u'\x07'     #  0x07 -> BELL
    u'\x08'     #  0x08 -> BACKSPACE
    u'\t'       #  0x09 -> HORIZONTAL TABULATION
    u'\n'       #  0x0A -> LINE FEED
    u'\x0b'     #  0x0B -> VERTICAL TABULATION
    u'\x0c'     #  0x0C -> FORM FEED
    u'\r'       #  0x0D -> CARRIAGE RETURN
    u'\x0e'     #  0x0E -> SHIFT OUT
    u'\x0f'     #  0x0F -> SHIFT IN
    u'\x10'     #  0x10 -> DATA LINK ESCAPE
    u'\x11'     #  0x11 -> DEVICE CONTROL ONE
    u'\x12'     #  0x12 -> DEVICE CONTROL TWO
    u'\x13'     #  0x13 -> DEVICE CONTROL THREE
    u'\x14'     #  0x14 -> DEVICE CONTROL FOUR
    u'\x15'     #  0x15 -> NEGATIVE ACKNOWLEDGE
    u'\x16'     #  0x16 -> SYNCHRONOUS IDLE
    u'\x17'     #  0x17 -> END OF TRANSMISSION BLOCK
    u'\x18'     #  0x18 -> CANCEL
    u'\x19'     #  0x19 -> END OF MEDIUM
    u'\x1a'     #  0x1A -> SUBSTITUTE
    u'\x1b'     #  0x1B -> ESCAPE
    u'\x1c'     #  0x1C -> FILE SEPARATOR
    u'\x1d'     #  0x1D -> GROUP SEPARATOR
    u'\x1e'     #  0x1E -> RECORD SEPARATOR
    u'\x1f'     #  0x1F -> UNIT SEPARATOR
    u' '        #  0x20 -> SPACE
    u'!'        #  0x21 -> EXCLAMATION MARK
    u'"'        #  0x22 -> QUOTATION MARK
    u'#'        #  0x23 -> NUMBER SIGN
    u'$'        #  0x24 -> DOLLAR SIGN
    u'%'        #  0x25 -> PERCENT SIGN
    u'&'        #  0x26 -> AMPERSAND
    u"'"        #  0x27 -> APOSTROPHE
    u'('        #  0x28 -> LEFT PARENTHESIS
    u')'        #  0x29 -> RIGHT PARENTHESIS
    u'*'        #  0x2A -> ASTERISK
    u'+'        #  0x2B -> PLUS SIGN
    u','        #  0x2C -> COMMA
    u'-'        #  0x2D -> HYPHEN-MINUS
    u'.'        #  0x2E -> FULL STOP
    u'/'        #  0x2F -> SOLIDUS
    u'0'        #  0x30 -> DIGIT ZERO
    u'1'        #  0x31 -> DIGIT ONE
    u'2'        #  0x32 -> DIGIT TWO
    u'3'        #  0x33 -> DIGIT THREE
    u'4'        #  0x34 -> DIGIT FOUR
    u'5'        #  0x35 -> DIGIT FIVE
    u'6'        #  0x36 -> DIGIT SIX
    u'7'        #  0x37 -> DIGIT SEVEN
    u'8'        #  0x38 -> DIGIT EIGHT
    u'9'        #  0x39 -> DIGIT NINE
    u':'        #  0x3A -> COLON
    u';'        #  0x3B -> SEMICOLON
    u'<'        #  0x3C -> LESS-THAN SIGN
    u'='        #  0x3D -> EQUALS SIGN
    u'>'        #  0x3E -> GREATER-THAN SIGN
    u'?'        #  0x3F -> QUESTION MARK
    u'@'        #  0x40 -> COMMERCIAL AT
    u'A'        #  0x41 -> LATIN CAPITAL LETTER A
    u'B'        #  0x42 -> LATIN CAPITAL LETTER B
    u'C'        #  0x43 -> LATIN CAPITAL LETTER C
    u'D'        #  0x44 -> LATIN CAPITAL LETTER D
    u'E'        #  0x45 -> LATIN CAPITAL LETTER E
    u'F'        #  0x46 -> LATIN CAPITAL LETTER F
    u'G'        #  0x47 -> LATIN CAPITAL LETTER G
    u'H'        #  0x48 -> LATIN CAPITAL LETTER H
    u'I'        #  0x49 -> LATIN CAPITAL LETTER I
    u'J'        #  0x4A -> LATIN CAPITAL LETTER J
    u'K'        #  0x4B -> LATIN CAPITAL LETTER K
    u'L'        #  0x4C -> LATIN CAPITAL LETTER L
    u'M'        #  0x4D -> LATIN CAPITAL LETTER M
    u'N'        #  0x4E -> LATIN CAPITAL LETTER N
    u'O'        #  0x4F -> LATIN CAPITAL LETTER O
    u'P'        #  0x50 -> LATIN CAPITAL LETTER P
    u'Q'        #  0x51 -> LATIN CAPITAL LETTER Q
    u'R'        #  0x52 -> LATIN CAPITAL LETTER R
    u'S'        #  0x53 -> LATIN CAPITAL LETTER S
    u'T'        #  0x54 -> LATIN CAPITAL LETTER T
    u'U'        #  0x55 -> LATIN CAPITAL LETTER U
    u'V'        #  0x56 -> LATIN CAPITAL LETTER V
    u'W'        #  0x57 -> LATIN CAPITAL LETTER W
    u'X'        #  0x58 -> LATIN CAPITAL LETTER X
    u'Y'        #  0x59 -> LATIN CAPITAL LETTER Y
    u'Z'        #  0x5A -> LATIN CAPITAL LETTER Z
    u'['        #  0x5B -> LEFT SQUARE BRACKET
    u'\\'       #  0x5C -> REVERSE SOLIDUS
    u']'        #  0x5D -> RIGHT SQUARE BRACKET
    u'^'        #  0x5E -> CIRCUMFLEX ACCENT
    u'_'        #  0x5F -> LOW LINE
    u'`'        #  0x60 -> GRAVE ACCENT
    u'a'        #  0x61 -> LATIN SMALL LETTER A
    u'b'        #  0x62 -> LATIN SMALL LETTER B
    u'c'        #  0x63 -> LATIN SMALL LETTER C
    u'd'        #  0x64 -> LATIN SMALL LETTER D
    u'e'        #  0x65 -> LATIN SMALL LETTER E
    u'f'        #  0x66 -> LATIN SMALL LETTER F
    u'g'        #  0x67 -> LATIN SMALL LETTER G
    u'h'        #  0x68 -> LATIN SMALL LETTER H
    u'i'        #  0x69 -> LATIN SMALL LETTER I
    u'j'        #  0x6A -> LATIN SMALL LETTER J
    u'k'        #  0x6B -> LATIN SMALL LETTER K
    u'l'        #  0x6C -> LATIN SMALL LETTER L
    u'm'        #  0x6D -> LATIN SMALL LETTER M
    u'n'        #  0x6E -> LATIN SMALL LETTER N
    u'o'        #  0x6F -> LATIN SMALL LETTER O
    u'p'        #  0x70 -> LATIN SMALL LETTER P
    u'q'        #  0x71 -> LATIN SMALL LETTER Q
    u'r'        #  0x72 -> LATIN SMALL LETTER R
    u's'        #  0x73 -> LATIN SMALL LETTER S
    u't'        #  0x74 -> LATIN SMALL LETTER T
    u'u'        #  0x75 -> LATIN SMALL LETTER U
    u'v'        #  0x76 -> LATIN SMALL LETTER V
    u'w'        #  0x77 -> LATIN SMALL LETTER W
    u'x'        #  0x78 -> LATIN SMALL LETTER X
    u'y'        #  0x79 -> LATIN SMALL LETTER Y
    u'z'        #  0x7A -> LATIN SMALL LETTER Z
    u'{'        #  0x7B -> LEFT CURLY BRACKET
    u'|'        #  0x7C -> VERTICAL LINE
    u'}'        #  0x7D -> RIGHT CURLY BRACKET
    u'~'        #  0x7E -> TILDE
    u'\x7f'     #  0x7F -> DELETE
    u'\x80'     #  0x80 -> <control>
    u'\x81'     #  0x81 -> <control>
    u'\x82'     #  0x82 -> <control>
    u'\x83'     #  0x83 -> <control>
    u'\x84'     #  0x84 -> <control>
    u'\x85'     #  0x85 -> <control>
    u'\x86'     #  0x86 -> <control>
    u'\x87'     #  0x87 -> <control>
    u'\x88'     #  0x88 -> <control>
    u'\x89'     #  0x89 -> <control>
    u'\x8a'     #  0x8A -> <control>
    u'\x8b'     #  0x8B -> <control>
    u'\x8c'     #  0x8C -> <control>
    u'\x8d'     #  0x8D -> <control>
    u'\x8e'     #  0x8E -> <control>
    u'\x8f'     #  0x8F -> <control>
    u'\x90'     #  0x90 -> <control>
    u'\x91'     #  0x91 -> <control>
    u'\x92'     #  0x92 -> <control>
    u'\x93'     #  0x93 -> <control>
    u'\x94'     #  0x94 -> <control>
    u'\x95'     #  0x95 -> <control>
    u'\x96'     #  0x96 -> <control>
    u'\x97'     #  0x97 -> <control>
    u'\x98'     #  0x98 -> <control>
    u'\x99'     #  0x99 -> <control>
    u'\x9a'     #  0x9A -> <control>
    u'\x9b'     #  0x9B -> <control>
    u'\x9c'     #  0x9C -> <control>
    u'\x9d'     #  0x9D -> <control>
    u'\x9e'     #  0x9E -> <control>
    u'\x9f'     #  0x9F -> <control>
    u'\xa0'     #  0xA0 -> NO-BREAK SPACE
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\xa4'     #  0xA4 -> CURRENCY SIGN
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\u060c'   #  0xAC -> ARABIC COMMA
    u'\xad'     #  0xAD -> SOFT HYPHEN
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\u061b'   #  0xBB -> ARABIC SEMICOLON
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\u061f'   #  0xBF -> ARABIC QUESTION MARK
    u'\ufffe'
    u'\u0621'   #  0xC1 -> ARABIC LETTER HAMZA
    u'\u0622'   #  0xC2 -> ARABIC LETTER ALEF WITH MADDA ABOVE
    u'\u0623'   #  0xC3 -> ARABIC LETTER ALEF WITH HAMZA ABOVE
    u'\u0624'   #  0xC4 -> ARABIC LETTER WAW WITH HAMZA ABOVE
    u'\u0625'   #  0xC5 -> ARABIC LETTER ALEF WITH HAMZA BELOW
    u'\u0626'   #  0xC6 -> ARABIC LETTER YEH WITH HAMZA ABOVE
    u'\u0627'   #  0xC7 -> ARABIC LETTER ALEF
    u'\u0628'   #  0xC8 -> ARABIC LETTER BEH
    u'\u0629'   #  0xC9 -> ARABIC LETTER TEH MARBUTA
    u'\u062a'   #  0xCA -> ARABIC LETTER TEH
    u'\u062b'   #  0xCB -> ARABIC LETTER THEH
    u'\u062c'   #  0xCC -> ARABIC LETTER JEEM
    u'\u062d'   #  0xCD -> ARABIC LETTER HAH
    u'\u062e'   #  0xCE -> ARABIC LETTER KHAH
    u'\u062f'   #  0xCF -> ARABIC LETTER DAL
    u'\u0630'   #  0xD0 -> ARABIC LETTER THAL
    u'\u0631'   #  0xD1 -> ARABIC LETTER REH
    u'\u0632'   #  0xD2 -> ARABIC LETTER ZAIN
    u'\u0633'   #  0xD3 -> ARABIC LETTER SEEN
    u'\u0634'   #  0xD4 -> ARABIC LETTER SHEEN
    u'\u0635'   #  0xD5 -> ARABIC LETTER SAD
    u'\u0636'   #  0xD6 -> ARABIC LETTER DAD
    u'\u0637'   #  0xD7 -> ARABIC LETTER TAH
    u'\u0638'   #  0xD8 -> ARABIC LETTER ZAH
    u'\u0639'   #  0xD9 -> ARABIC LETTER AIN
    u'\u063a'   #  0xDA -> ARABIC LETTER GHAIN
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\u0640'   #  0xE0 -> ARABIC TATWEEL
    u'\u0641'   #  0xE1 -> ARABIC LETTER FEH
    u'\u0642'   #  0xE2 -> ARABIC LETTER QAF
    u'\u0643'   #  0xE3 -> ARABIC LETTER KAF
    u'\u0644'   #  0xE4 -> ARABIC LETTER LAM
    u'\u0645'   #  0xE5 -> ARABIC LETTER MEEM
    u'\u0646'   #  0xE6 -> ARABIC LETTER NOON
    u'\u0647'   #  0xE7 -> ARABIC LETTER HEH
    u'\u0648'   #  0xE8 -> ARABIC LETTER WAW
    u'\u0649'   #  0xE9 -> ARABIC LETTER ALEF MAKSURA
    u'\u064a'   #  0xEA -> ARABIC LETTER YEH
    u'\u064b'   #  0xEB -> ARABIC FATHATAN
    u'\u064c'   #  0xEC -> ARABIC DAMMATAN
    u'\u064d'   #  0xED -> ARABIC KASRATAN
    u'\u064e'   #  0xEE -> ARABIC FATHA
    u'\u064f'   #  0xEF -> ARABIC DAMMA
    u'\u0650'   #  0xF0 -> ARABIC KASRA
    u'\u0651'   #  0xF1 -> ARABIC SHADDA
    u'\u0652'   #  0xF2 -> ARABIC SUKUN
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
    u'\ufffe'
)

### Encoding table
encoding_table=codecs.charmap_build(decoding_table)
