package com.gmail.doloiu22.controller;

import com.gmail.doloiu22.model.SuspendedUserEntity;
import com.gmail.doloiu22.model.UserEntity;
import com.gmail.doloiu22.repository.SuspendedUserRepository;
import com.gmail.doloiu22.repository.UserRepository;
import com.gmail.doloiu22.service.SuspendedUserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.Authentication;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;

import java.time.LocalDate;
import java.util.List;

@Controller
@RequestMapping("/admin")
public class AdminController {

    @Autowired
    private UserRepository userRepository;
    @Autowired
    private SuspendedUserService suspendedUserService;


    @GetMapping
    public String open(Model model, Authentication authentication) {

        List<UserEntity> userAccounts = userRepository.findAll();

        model.addAttribute("userAccounts", userAccounts);

        return "admin/dashboard";
    }

    @PostMapping("/changePassword")
    public String changePassword(@RequestParam("userID") Long userID, @RequestParam("newPassword") String newPassword) {

        UserEntity userEntity = userRepository.findById(userID).get();
        userEntity.setPassword(new BCryptPasswordEncoder().encode(newPassword));
        userRepository.save(userEntity);

        return "others/user_updated";
    }

    @PostMapping("/suspendAccount")
    public String suspendAccount(@RequestParam("userID") Long userID, @RequestParam("suspensionDate")LocalDate date) {

        UserEntity userEntity = userRepository.findById(userID).get();

        if (userEntity.getLicensePlate().equals("ADMIN"))
            return "other_errors/protected_account";

        SuspendedUserEntity suspendedUser = new SuspendedUserEntity();
        suspendedUser.setUserID(userID);
        suspendedUser.setSuspensionEndDate(date);

        if (!suspendedUserService.isUserIDSuspended(userID))
            suspendedUserService.getRepository().save(suspendedUser);
        else return "other_errors/user_suspended";

        return "others/user_updated";
    }

    @PostMapping("/deleteAccount")
    public String deleteAccount(@RequestParam("userID") Long userID) {

        UserEntity userEntity = userRepository.findById(userID).get();

        if (userEntity.getLicensePlate().equals("ADMIN") || userEntity.getLicensePlate().equals("BV20WXP"))
            return "other_errors/protected_account";

        userRepository.delete(userEntity);

        return "others/user_updated";
    }

}
