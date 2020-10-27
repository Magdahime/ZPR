test = ARGUMENTS.get('test', 1)

if not int(test):
    print("Not testing tonight!")
    SConscript('src/SConscript', variant_dir='build', duplicate = 0)
else:
    print("Time for some tests!")
    SConscript('tests/SConscript', variant_dir='build', duplicate = 0)