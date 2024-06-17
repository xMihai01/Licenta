package com.gmail.doloiu22.controller;

import com.gmail.doloiu22.model.SessionEntity;
import com.gmail.doloiu22.model.UserEntity;
import com.gmail.doloiu22.service.SessionService;
import com.gmail.doloiu22.util.Utils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.Authentication;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.*;

import java.time.LocalDateTime;
import java.util.Objects;
import java.util.Optional;

@Controller
@RequestMapping("/stuck")
public class StuckController {

    @Autowired
    private SessionService sessionService;

    @GetMapping()
    public String open(Model model, Authentication authentication) {
        return "stuck";
    }

    @PostMapping()
    public String confirm(@RequestParam("secretID") String secretID, @RequestParam("cameraID") String cameraID){

        Optional<SessionEntity> session = sessionService.findBySecretID(secretID);
        if (!session.isPresent()) {
            return "other_errors/invalid_secretid";
        }

        SessionEntity validSession = session.get();
        if (!sessionService.isSecretIDValid(validSession, false)) {
            return "other_errors/invalid_secretid";
        }

        try {
            long cameraIDasLong = Long.parseLong(cameraID);

            if (cameraID.length() == 0) {
                throw new NumberFormatException();
            }
        }catch (NumberFormatException e) {
            return "other_errors/invalid_secretid";
        }

        validSession.setExitTime(LocalDateTime.now());
        sessionService.save(validSession);

        return "others/stuck_success";
        }

}
