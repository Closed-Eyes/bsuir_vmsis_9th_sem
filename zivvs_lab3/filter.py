content_file = open("test_text.txt", "r")

content = content_file.read()

lowercase_content = content.decode("utf-8").lower()

filtered_content = re.findall(u"[а-я]+",lowercase_content)

#print "".join(filtered_content)

result = "".join(filtered_content)

out_file = open("filtered_text.txt", "w")
out_file.write(result.encode("utf-8"))
