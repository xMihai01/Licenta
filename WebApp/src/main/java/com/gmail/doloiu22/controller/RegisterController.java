package com.gmail.doloiu22.controller;

import com.gmail.doloiu22.config.CustomAuthenticationSuccessHandler;
import com.gmail.doloiu22.model.UserEntity;
import com.gmail.doloiu22.service.UserService;
import com.gmail.doloiu22.service.UserValidatorService;
import com.gmail.doloiu22.util.Utils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.userdetails.User;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.*;

@Controller
@RequestMapping("/register")
public class RegisterController {
    private UserService userService;
    private UserValidatorService userValidatorService;

    @Autowired
    public RegisterController(UserService userService, UserValidatorService userValidatorService) {
        this.userService = userService;
        this.userValidatorService = userValidatorService;
    }

    @GetMapping()
    public String open(Model model){
        model.addAttribute("user", new UserEntity());
        return "register";
    }

    @PostMapping()
    public String register(@RequestParam("secretID") String secretID,
                           @ModelAttribute("user") UserEntity user, BindingResult bindingResult){

        user.setLicensePlate(Utils.getLicensePlateFromSecretID(secretID));
        userValidatorService.validate(user, bindingResult);

        if(bindingResult.hasErrors())
            return "register";

        if (userService.register(user, secretID))
            return "/login";
        else return "register";
    }
}
