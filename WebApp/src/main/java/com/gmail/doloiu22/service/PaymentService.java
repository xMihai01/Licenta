package com.gmail.doloiu22.service;

import com.gmail.doloiu22.model.PaymentEntity;
import com.gmail.doloiu22.model.SessionEntity;
import com.gmail.doloiu22.repository.PaymentRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;
import java.util.Optional;

@Service
public class PaymentService {

    public static float PRICE_PER_HOUR = 3.0f;
    public static float CONVERSION_TO_PREFFERED_CURRENCY = 5.0f;

    @Autowired
    private PaymentRepository paymentRepository;
    @Autowired
    private SessionService sessionService;

    public boolean isSessionPaid(Long sessionID) {
        Optional<PaymentEntity> payment = paymentRepository.findBySessionID(sessionID);
        Optional<SessionEntity> session = sessionService.findById(sessionID);
        if (session.get().didItExit())
            return true;
        if (payment.isPresent())
            return payment.get().getAllowedLeavingTime().isAfter(LocalDateTime.now());
        return false;
    }

    public Optional<PaymentEntity> findBySessionID(Long sessionID) {
        return paymentRepository.findBySessionID(sessionID);
    }

    public PaymentRepository getPaymentRepository() { return paymentRepository;}

}
