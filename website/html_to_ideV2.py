with open("website.h", 'w', encoding="utf8") as output_file:
    #output_file.write('#include <avr/pgmspace.h>\n') PROGMEM
    #output_file.write('const char website[] = ')
    #output_file.write('#define WEBSITE ')
    with open('website/website.html', 'r', encoding="utf8") as input_file:
        output_file.write('#include <Ethernet.h>\n')
        output_file.write('void website(EthernetClient &client, uint8_t IPaddr[], uint8_t inputNumber[], const char* status, const char* save, const char* p_frq_checked[], uint8_t brightness[], const char* version){')
        output_file.write('\n')  
        x = 0
        for line in input_file.readlines():
            output_file.write('     client.print("')                      
            if x == 2:
                x = 0
            if line.find("/*") >= 0:
                x = 1
            if line.find("*/") >= 0:
                x = 2
            if x == 0:
                line = line.replace('	', '')                
                line = line.replace('\n', '')
                line = line.replace('"', '\\"')
                line = line.replace('°', '");\n     client.print(')
                line = line.replace('`', ');\n     client.print(".");\n     client.print(')
                line = line.replace('^', ');\n     client.print("')
                output_file.write(line) 
                output_file.write('");')
                output_file.write('\n')
        output_file.write('}')
        #~a`b`c`d^
        #<p id="connected">CONNECTED</p>
