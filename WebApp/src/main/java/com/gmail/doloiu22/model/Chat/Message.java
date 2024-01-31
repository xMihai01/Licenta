package com.gmail.doloiu22.model.Chat;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public class Message {

    private String message;
    private String sender;
    private LocalDateTime timeWhenMessageWasSent;


    public Message(String message, String sender, LocalDateTime timeWhenMessageWasSent) {
        this.message = message;
        this.sender = sender;
        this.timeWhenMessageWasSent = timeWhenMessageWasSent;
    }

    public LocalDateTime getTimeWhenMessageWasSent() {
        return timeWhenMessageWasSent;
    }
    public String getCleanTimeWhenMessageWasSent() {
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm");
        return timeWhenMessageWasSent.format(formatter);
    }

    public void setTimeWhenMessageWasSent(LocalDateTime timeWhenMessageWasSent) {
        this.timeWhenMessageWasSent = timeWhenMessageWasSent;
    }

    public String getSender() {
        return sender;
    }

    public void setSender(String sender) {
        this.sender = sender;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }
}
