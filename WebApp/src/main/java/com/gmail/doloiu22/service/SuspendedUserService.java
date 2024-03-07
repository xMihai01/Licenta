package com.gmail.doloiu22.service;

import com.gmail.doloiu22.model.SuspendedUserEntity;
import com.gmail.doloiu22.repository.SuspendedUserRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.time.LocalDate;
import java.util.List;
import java.util.Optional;

@Service
public class SuspendedUserService {

    private final SuspendedUserRepository suspendedUserRepository;

    @Autowired
    public SuspendedUserService(SuspendedUserRepository suspendedUserRepository) {
        this.suspendedUserRepository = suspendedUserRepository;
    }

    public List<SuspendedUserEntity> findAllByUserID(Long userID) {
        return suspendedUserRepository.findAllByUserID(userID);
    }

    public boolean isUserIDSuspended(Long userID) {
        List<SuspendedUserEntity> suspensions = suspendedUserRepository.findAllByUserID(userID);

        for (SuspendedUserEntity suspendedUser : suspensions) {
            if (suspendedUser.getSuspensionEndDate().isAfter(LocalDate.now()))
                return true;
        }
        return false;
    }

    public LocalDate getEndingTimeOfSuspensionForUserID(Long userID) {
        List<SuspendedUserEntity> suspensions = suspendedUserRepository.findAllByUserID(userID);

        for (SuspendedUserEntity suspendedUser : suspensions) {
            if (suspendedUser.getSuspensionEndDate().isAfter(LocalDate.now()))
                return suspendedUser.getSuspensionEndDate();
        }
        return null;
    }
    public SuspendedUserRepository getRepository() {
        return suspendedUserRepository;
    }

}
