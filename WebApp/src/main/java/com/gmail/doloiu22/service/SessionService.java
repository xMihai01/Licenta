package com.gmail.doloiu22.service;

import com.gmail.doloiu22.model.SessionEntity;
import com.gmail.doloiu22.repository.SessionRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;
import java.util.Optional;

@Service
public class SessionService {
    private final SessionRepository sessionRepository;

    private final LocalDateTime NOTIME = LocalDateTime.of(
            1970, 1, 1, 2, 0, 0, 0);
    private final int MINIMUM_LICENSE_PLATE_LENGTH = 5;

    @Autowired
    public SessionService(SessionRepository sessionRepository) {
        this.sessionRepository = sessionRepository;
    }

    Optional<SessionEntity> findBySecretID(String secretID) {
        return sessionRepository.findBySecretID(secretID);
    }

    boolean isSecretIDValid(SessionEntity sessionEntity) {
        return sessionEntity.getExitTime().isEqual(NOTIME) && sessionEntity.getLicensePlate().length() > MINIMUM_LICENSE_PLATE_LENGTH;
    }
}
