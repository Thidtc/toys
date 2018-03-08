import re
with open('res1.txt', "r") as f1, open('res2.txt', "r") as f2:
    line1 = f1.readline()
    line2 = f2.readline()
    def get(line):
        return re.split(r'\(|\)| |,', line))
    x1, y1, _,r1, g1, b1 = get(line1)
    x2, y2, _,r2, g2, b2 = get(line2)
    assert x1 == x2 and y1 == y2
    if r1 != r2 or g1 != g2 or b1 != b2:
        print line1, '   ...   ', line2
