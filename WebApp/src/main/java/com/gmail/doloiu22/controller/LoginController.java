package com.gmail.doloiu22.controller;

import org.springframework.security.core.Authentication;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

@Controller
@RequestMapping("/login")
public class LoginController {

    @GetMapping
    public String open(){
        return "login";
    }

    @GetMapping("/forgotPassword")
    public String openForgotPassword(Model model, Authentication authentication) {
        return "forgotPassword";
    }

    @RequestMapping("/error")
    public String openLoginError() {
        return "other_errors/login_invalid_details";
    }

}