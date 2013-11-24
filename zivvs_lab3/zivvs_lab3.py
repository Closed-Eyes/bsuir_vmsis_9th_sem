from pprint import pprint
import operator
import bitstring

def getSize(fileobject):
    fileobject.seek(0,2) # move the cursor to the end of the file
    size = fileobject.tell()
    return size


if __name__ == '__main__':
	generated_codes = []
	separate_code = '10'

	# generate 28 codes for 28 english letters
	code_length = 1
	
	while len(generated_codes) < 29:
		code_candidates = [bin(x)[2:].rjust(code_length, '0') for x in range(2**code_length)]
		
		for candidate in code_candidates:
			if not separate_code in candidate:
				generated_codes.append(candidate)
		
		code_length += 1
	print "Generated codes:"
	pprint(generated_codes)
	#print len(generated_codes)

	input_file = open("filtered_text.txt", "r")
	content = input_file.read().decode("utf-8")

	alphabet = {}


	#print content
	for char in content:
		if not char in alphabet:
			alphabet[char] = 1
		else:
			alphabet[char] += 1

	output_alphabet = {}
	input_alphabet = {}

	while len(alphabet) > 0:
		most_even_char = max(alphabet.iteritems(), key=operator.itemgetter(1))[0]

		output_alphabet[most_even_char] = generated_codes[0]
		input_alphabet[generated_codes[0]] = most_even_char
		del alphabet[most_even_char]
		generated_codes = generated_codes[1:]

	print "\nApplied codes for characters:"
	for key in output_alphabet:
		print key, '=', output_alphabet[key]
	
	output_content = []

	for char in content:
		output_content.append(output_alphabet[char])
		output_content.append(separate_code)

	result_bit_array = "".join(output_content)

	asd = bitstring.BitArray("0b" + result_bit_array)
	onetwo = asd.unpack('bytes')
	#print onetwo

	output_file = open("outfile.bin", "wb")
	output_file.write(onetwo[0])
	print "\nStatistics:"	
	print "Raw file size:     ", getSize(input_file), "bytes"
	print "Encoded file size: ", getSize(output_file), "bytes"

	output_file.close()
	input_file.close()


	# SECOND PART - DECODING

	input_file = open("outfile.bin", "rb")

	content = input_file.read()
	
	bits = bitstring.pack("bytes", content)
	bit_str = bits.bin
	characters = bit_str.split(separate_code)
	
	original_text = ""
	for char in characters:
		if char in input_alphabet:
			original_text += input_alphabet[char]

	print "\n"
	print "Start of decoded text:"
	print original_text[:76], "..."
	print 
	print "Start of original_text:"
	print open("filtered_text.txt", "r").read().decode("utf-8")[:76], "..."

	open("decoded.txt", "w").write(original_text.encode("utf-8"))


