#!/usr/bin/python

def get_data():
    prev_char = 0
    upper_ints = []
    lower_ints = []
    with open("UnicodeData.txt") as data:
        for line in data:
            tokens = line.split(";")
            char_int = int(tokens[0], 16)

            upper = tokens[12]
            lower = tokens[13]
            if len(upper):
                upper_int = int(upper, 16)
            else:
                upper_int = char_int
            if len(lower):
                lower_int = int(lower, 16)
            else:
                lower_int = char_int

            while (prev_char < char_int - 1):
                prev_char +=1
                if (prev_char <= 65535):
                    upper_ints.append(prev_char);
                    lower_ints.append(prev_char);
            if char_int > 65535:
                break
            upper_ints.append(upper_int);
            lower_ints.append(lower_int);
            prev_char = char_int

    return [upper_ints, lower_ints]

def print_needy(characters):
    for i in range(0, len(characters)):
        if characters[i] != i:
            print str(i),

def get_max_diff(characters):
    max_diff = 0
    max_diff_at = 0
    for i in range(0, len(characters)):
        diff = abs(characters[i] - i)
        if diff > max_diff:
            max_diff = diff
            max_diff_at = i
    return max_diff_at

def get_array(case, lowest, highest, all):
    string = "    static const Utf16 %s%d[] = {" % (case, lowest)
    for i in range(lowest, highest+1):
        string += "%d," % all[i]
    string = string[0:-1]
    string += "};\n";
    return string

def get_if_clause(case, lowest, highest, all):
    string = "    if (chr >= %d && chr <= %d)\n" % (lowest, highest)
    string += "        return %s%d[chr-%d];\n" % (case, lowest, lowest)
    return string

if __name__ == '__main__':
    upper_ints, lower_ints = get_data()

    print_needy(upper_ints)
    print
    print
    print_needy(lower_ints)
    print
    print "Max diff upper", get_max_diff(upper_ints)
    print "Max diff lower", get_max_diff(lower_ints)

    with open("../../src/uBase/UnicodeUpperLower.inc", 'w') as UpperLower_inc:
        upper_ints, lower_ints = get_data()

        UpperLower_inc.write('Utf16 Unicode::ToUpper(Utf16 chr)\n{\n')
        ranges = ((97,1414), (7545,9449), (11312,11565), (42561,42921))
        for rng in ranges:
            UpperLower_inc.write(get_array('u', rng[0], rng[1], upper_ints))
        for rng in ranges:
            UpperLower_inc.write(get_if_clause('u', rng[0], rng[1], upper_ints))
        UpperLower_inc.write('    else\n        return chr;\n')
        UpperLower_inc.write('}\n\n')

        UpperLower_inc.write('Utf16 Unicode::ToLower(Utf16 chr)\n{\n')
        ranges = ((65,1366), (4256,4301), (7680,9423), (11246,11506), (42560,42922), (65313,65338))
        for rng in ranges:
            UpperLower_inc.write(get_array('l', rng[0], rng[1], lower_ints))
        for rng in ranges:
            UpperLower_inc.write(get_if_clause('l', rng[0], rng[1], lower_ints))
        UpperLower_inc.write('    else\n        return chr;\n')
        UpperLower_inc.write('}')
