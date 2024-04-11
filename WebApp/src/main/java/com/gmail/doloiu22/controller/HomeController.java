package com.gmail.doloiu22.controller;
import com.gmail.doloiu22.config.util.Role;
import com.gmail.doloiu22.model.ParkingSessionEntity;
import com.gmail.doloiu22.model.PaymentEntity;
import com.gmail.doloiu22.model.SessionEntity;
import com.gmail.doloiu22.service.ParkingSessionService;
import com.gmail.doloiu22.service.PaymentService;
import com.gmail.doloiu22.service.SessionService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;
import com.gmail.doloiu22.util.*;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Optional;

@Controller
@RequestMapping("/home")
public class HomeController {

    @Autowired
    private SessionService sessionService;
    @Autowired
    private ParkingSessionService parkingSessionService;
    @Autowired
    private PaymentService paymentService;

    @GetMapping
    public String open(Model model, Authentication authentication){
        List<SessionEntity> sessions = sessionService.findAllByLicensePlate(authentication.getName());
        Collections.reverse(sessions);

        model.addAttribute("isAdmin", authentication.getAuthorities().contains(new SimpleGrantedAuthority(Role.ADMIN.name())));
        model.addAttribute("paymentService", paymentService);
        model.addAttribute("sessions", sessions);

        return "home/dashboard";
    }

    @GetMapping("/spaces/{sessionID}")
    public String openSpaces(@PathVariable int sessionID, Model model, Authentication authentication) {

        List<ParkingSessionEntity> sessions = parkingSessionService.findAllBySessionID((long) sessionID);
        Optional<SessionEntity> session = sessionService.findById((long)sessionID);
        if (session.isEmpty() || !session.get().getLicensePlate().equals(authentication.getName()))
            return "other_errors/missing_permission";

        model.addAttribute("sessions", sessions);
        model.addAttribute("sessionID", sessionID);
        model.addAttribute("parkingSessionService", parkingSessionService);
        model.addAttribute("isAdmin", authentication.getAuthorities().contains(new SimpleGrantedAuthority(Role.ADMIN.name())));

        return "home/spaces";
    }

    @GetMapping("/payment/{sessionID}")
    public String openPaymentPage(@PathVariable int sessionID, Model model, Authentication authentication) {

        Optional<SessionEntity> session = sessionService.findById((long)sessionID);
        if (paymentService.isSessionPaid((long)sessionID))
            return "other_errors/session_already_paid";
        if (session.isEmpty() || !session.get().getLicensePlate().equals(authentication.getName()))
            return "other_errors/missing_permission";


        float priceToPay = session.get().getTimeSpentAsLong()/60.0f * PaymentService.PRICE_PER_HOUR * PaymentService.CONVERSION_TO_PREFFERED_CURRENCY;

        model.addAttribute("sessionID", sessionID);
        model.addAttribute("priceToPay", priceToPay);
        model.addAttribute("isAdmin", authentication.getAuthorities().contains(new SimpleGrantedAuthority(Role.ADMIN.name())));

        return "home/payment";
    }

    @PostMapping("/viewSpaceHistory")
    public String viewParkingHistoryForSessionIDClick(@RequestParam("sessionID") Long sessionID) {

        return "redirect:/home/spaces/" + sessionID.toString();
    }
    @PostMapping("/viewPayment")
    public String viewPaymentForSessionIDClick(@RequestParam("sessionID") Long sessionID) {

        return "redirect:/home/payment/" + sessionID.toString();
    }
    @PostMapping("/createReport")
    public String createReportForSession(@RequestParam("sessionID") Long sessionID) {

        return "redirect:/report/create/" + sessionID.toString();
    }
    @PostMapping("/confirmPayment")
    public String confirmPayment(@RequestParam("sessionID") Long sessionID) {

        PaymentEntity paymentEntity = new PaymentEntity();

        if (paymentService.findBySessionID(sessionID).isPresent())
            paymentEntity.setId(paymentService.findBySessionID(sessionID).get().getId());

        paymentEntity.setSessionID(sessionID);
        paymentEntity.setAllowedLeavingTime(LocalDateTime.now().plusMinutes(15));

        paymentService.getPaymentRepository().save(paymentEntity);

        return "others/payment_success";
    }

}