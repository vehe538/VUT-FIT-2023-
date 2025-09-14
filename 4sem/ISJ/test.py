teststr1 = 'Ada;Bob;Bob;Ada;Cyril;Ada;Cyril'
teststr2 = 'Dan@Ada@Ada@Dan@Cyril@Ed@Frank'

used_by_both = []

for name in teststr1.split(";"):
    if name in teststr2.split("@") and name not in used_by_both:
        used_by_both.append(name)

for ev in used_by_both:
    print(ev)
