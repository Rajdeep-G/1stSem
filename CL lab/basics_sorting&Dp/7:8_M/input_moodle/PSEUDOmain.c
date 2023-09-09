main(argc, argv):
    if argc is not 3:
        print error message - "Usage: <program name> <algorithm (a/b)> <input_file>"
    
    algorithm = first character of argv[1]
    input_file = argv[2]
    
    file = open input_file
    if file is not found:
        print error message - "Error: Cannot open input file."
    
    n = read integer from file
    presentations = allocate memory for n `Presentation` structures
    
    for i in range(n):
        read start, end, popularity, title from file and store in each `Presentation` structure
        Hint: use `fgets` to read string 
    
    close file
    
    switch algorithm:
        case 'a':
            call greedy_start(presentations, n)
            break
        case 'b':
            call greedy_popularity(presentations, n)
            break
        default:
            print "Error: Invalid algorithm option."
    
    free memory allocated for presentations
    return 0