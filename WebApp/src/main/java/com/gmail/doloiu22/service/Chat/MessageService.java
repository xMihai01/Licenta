package com.gmail.doloiu22.service.Chat;

import com.gmail.doloiu22.model.Chat.Message;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.time.LocalDateTime;
import java.util.LinkedList;
import java.util.List;

public class MessageService {

    private static final String globalMessagesJsonFileName = "globalMessage.json";
    private static final String textMessageCell = "text";
    private static final String senderNameCell = "sender";
    private static final String timeSentCell = "timeSent";

    public List<Message> getAllMessages() {
        List<Message> allMessages = new LinkedList<>();

        JSONArray messagesList = getAllMessageAsJSONArray();
        for (Object message : messagesList) {
            allMessages.add(parseMessageJsonObject((JSONObject) message));
        }
        return allMessages;
    }

    private JSONArray getAllMessageAsJSONArray() {
        JSONParser jsonParser = new JSONParser();

        try (FileReader reader = new FileReader(globalMessagesJsonFileName)) {
            Object obj = jsonParser.parse(reader);

            if (obj instanceof JSONArray) {
                JSONArray messagesList = (JSONArray) obj;
                return messagesList;
            } else {
                System.err.println("Invalid JSON file format. Expected an array.");
            }
        } catch (IOException | ParseException e) {
            e.printStackTrace();
        }
        return new JSONArray();
    }

    private Message parseMessageJsonObject(JSONObject message) {
        return new Message(message.get(textMessageCell).toString(), message.get(senderNameCell).toString(), LocalDateTime.parse(message.get(timeSentCell).toString()));
    }

    public void sendMessage(Message message) {
        String sender = message.getSender();
        String text = message.getMessage();
        LocalDateTime time = message.getTimeWhenMessageWasSent();

        JSONArray previousMessages = getAllMessageAsJSONArray();

        JSONObject messageDetails = new JSONObject();
        messageDetails.put(senderNameCell, sender);
        messageDetails.put(textMessageCell, text);
        messageDetails.put(timeSentCell, time.toString());

        previousMessages.add(messageDetails);

        try (FileWriter file = new FileWriter(globalMessagesJsonFileName)) {
            file.write(previousMessages.toJSONString());
            file.flush();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
