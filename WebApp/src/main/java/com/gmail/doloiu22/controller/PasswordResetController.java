package com.gmail.doloiu22.controller;

import com.gmail.doloiu22.model.SessionEntity;
import com.gmail.doloiu22.model.UserEntity;
import com.gmail.doloiu22.repository.UserRepository;
import com.gmail.doloiu22.service.SessionService;
import com.gmail.doloiu22.util.Utils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.Authentication;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.*;

import java.util.Objects;
import java.util.Optional;

@Controller
@RequestMapping("/passwordReset")
public class PasswordResetController {

    @Autowired
    private UserRepository userRepository;
    @Autowired
    private SessionService sessionService;

    @GetMapping()
    public String open(Model model){
        model.addAttribute("user", new UserEntity());
        return "forgotPassword";
    }

    @PostMapping()
    public String forgotPassword(@RequestParam("secretID") String secretID, @RequestParam("licensePlate") String licensePlate,
                           @ModelAttribute("user") UserEntity user, BindingResult bindingResult){

        if (!Objects.equals(licensePlate, Utils.getLicensePlateFromSecretID(secretID))) {
            return "other_errors/invalid_licensePlate";
        }

        Optional<SessionEntity> sessionEntity = sessionService.findBySecretID(secretID);

        // check if this is also the last entry session
        if (!sessionEntity.isPresent()) {

            return "other_errors/invalid_licensePlate";
        }
        UserEntity userEntity = userRepository.findByLicensePlate(licensePlate).get();
        userEntity.setPassword(new BCryptPasswordEncoder().encode(user.getPassword()));
        userRepository.save(userEntity);

        return "others/passwordReset_success";
    }

}
