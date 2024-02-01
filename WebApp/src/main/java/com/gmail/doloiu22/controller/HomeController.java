package com.gmail.doloiu22.controller;
import com.gmail.doloiu22.config.util.Role;
import com.gmail.doloiu22.model.SessionEntity;
import com.gmail.doloiu22.service.SessionService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
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

        model.addAttribute("isAdmin", authentication.getAuthorities().contains(new SimpleGrantedAuthority(Role.ADMIN.name())));
        model.addAttribute("sessions", sessions);

        return "home/dashboard";
    }

    @GetMapping("/spaces/{sessionID}")
    public String openSpaces(@PathVariable int sessionID, Model model, Authentication authentication) {
        List<SessionEntity> sessions = sessionService.findAllByLicensePlate(authentication.getName());
        Optional<SessionEntity> session = sessionService.findById((long)sessionID);
        if (session.isEmpty() || !session.get().getLicensePlate().equals(authentication.getName()))
            return "other_errors/missing_permission";

        model.addAttribute("sessions", sessions);
        model.addAttribute("sessionID", sessionID);
        model.addAttribute("isAdmin", authentication.getAuthorities().contains(new SimpleGrantedAuthority(Role.ADMIN.name())));

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
    @PostMapping("/createReport")
    public String createReportForSession(@RequestParam("sessionID") Long sessionID) {

        return "redirect:/report/create/" + sessionID.toString();
    }

}