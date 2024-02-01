package com.gmail.doloiu22.controller;

import com.gmail.doloiu22.config.util.Role;
import com.gmail.doloiu22.model.Chat.Message;
import com.gmail.doloiu22.model.SessionEntity;
import com.gmail.doloiu22.service.Chat.MessageService;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

@Controller
@RequestMapping("/chat")
public class ChatController {

    private final MessageService messageService = new MessageService();
    private String currentUser = "";

    @GetMapping()
    public String open(Model model, Authentication authentication){

        List<Message> messages = messageService.getAllMessages();
        Collections.reverse(messages);

        currentUser = authentication.getName();

        model.addAttribute("connectedAs", currentUser);
        model.addAttribute("messages", messages);
        model.addAttribute("isAdmin", authentication.getAuthorities().contains(new SimpleGrantedAuthority(Role.ADMIN.name())));

        return "chat/chat";
    }

    @GetMapping()
    @RequestMapping("/send")
    public String openChatPost(Model model, Authentication authentication) {

        return "chat/send";
    }

    @PostMapping("/sendMessage")
    public String sendMessage(@RequestParam("message") String message) {

        if (message.length() > 0) {
            messageService.sendMessage(new Message(message, currentUser, LocalDateTime.now()));
        } else {
            return "other_errors/empty_message";
        }

        return "redirect:/chat";
    }

}
