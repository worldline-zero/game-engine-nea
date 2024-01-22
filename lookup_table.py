i = ord('A')

while i <= ord('Z'):
    print("{ '"+chr(i)+"' , glm::vec2("+str(((i-ord('A')) % 8)/8.0)+"f, "+str(((i-ord('A'))//8)/8.0)+"f) },")
    i += 1
print("{ ' ' , glm::vec2("+str(((i-ord('A')) % 8)/8.0)+"f, "+str(((i-ord('A'))//8)/8.0)+"f) },")
i += 1

for j in range(1, 10):
    print("{ '"+str(j)+"' , glm::vec2("+str(((i-ord('A')) % 8)/8.0)+"f, "+str(((i-ord('A'))//8)/8.0)+"f) },")
    i += 1
print("{ '0' , glm::vec2("+str(((i-ord('A')) % 8)/8.0)+"f, "+str(((i-ord('A'))//8)/8.0)+"f) },")
i += 1
print("{ '.' , glm::vec2("+str(((i-ord('A')) % 8)/8.0)+"f, "+str(((i-ord('A'))//8)/8.0)+"f) },")
i += 1
print("{ ',' , glm::vec2("+str(((i-ord('A')) % 8)/8.0)+"f, "+str(((i-ord('A'))//8)/8.0)+"f) },")
i += 1
print("{ '!' , glm::vec2("+str(((i-ord('A')) % 8)/8.0)+"f, "+str(((i-ord('A'))//8)/8.0)+"f) },")
i += 1
