def commandToCan_Convertion(message):
    can_id, command = message.split(":")
    mapping = {
        "Launch_Rocket": [8, 7, 6, 5, 4, 3, 2, 1],
        "Terminate Flight": [255, 0, 255, 0, 255, 0, 255, 0]
    }
    bytes_list = mapping.get(command)
    if bytes_list is None:
        raise ValueError(f"Ukjent kommando: {command}")
    return f"{can_id}:{':'.join(map(str, bytes_list))}\n"


def canTocommand_convertion(text):
    bitListe = list()    

    can_id, content = text.split(":") #Splits string
    intCan_id = int(can_id) 
    intContent = int(content)

    result = {"id": intCan_id}

    #------------Check-type-----------------------
    if intCan_id == 200: message_type = "status"
    else: message_type = "float"
    
    #------------Result-based-on-type-------------
    #1x 8/64 Byte/bit ---- float value
    if message_type == 'float': 
        result["value"] = intContent

    #1x 8/64 Byte/bit ---- status bits 
    elif message_type == 'status': 
        for i in range(63):
            bitListe.append((intContent >> (63 - i)) & 1)
        result["value"] = bitListe  
    else:
        raise ValueError("Ukjent message_type. Bruk 'float' eller 'status'.")
    return result
