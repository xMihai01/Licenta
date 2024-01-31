package com.gmail.doloiu22.controller;
import com.gmail.doloiu22.model.SessionEntity;
import com.gmail.doloiu22.service.SessionService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.Authentication;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;
import com.gmail.doloiu22.util.*;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

@Controller
@RequestMapping("/home")
public class HomeController {

    @Autowired
    private SessionService sessionService;

    @GetMapping
    public String open(Model model, Authentication authentication){
        List<SessionEntity> sessions = sessionService.findAllByLicensePlate(authentication.getName());

        model.addAttribute("sessions", sessions);

        return "home/dashboard";
    }

    @GetMapping("/spaces/{sessionID}")
    public String openSpaces(@PathVariable int sessionID, Model model, Authentication authentication) {
        List<SessionEntity> sessions = sessionService.findAllByLicensePlate(authentication.getName());

        model.addAttribute("sessions", sessions);
        model.addAttribute("sessionID", sessionID);

        return "home/spaces";
    }

    @PostMapping("/viewSpaceHistory")
    public String viewParkingHistoryForSessionIDClick(@RequestParam("sessionID") Long sessionID) {

        return "redirect:/home/spaces/" + sessionID.toString();
    }
    @PostMapping("/viewPayment")
    public String viewPaymentForSessionIDClick(@RequestParam("sessionID") Long sessionID) {

        return "redirect:/home/spaces/" + "222";
    }

}