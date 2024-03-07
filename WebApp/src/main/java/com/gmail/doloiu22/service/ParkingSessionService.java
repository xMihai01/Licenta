package com.gmail.doloiu22.service;

import com.gmail.doloiu22.model.ParkingSessionEntity;
import com.gmail.doloiu22.model.ParkingSpaceEntity;
import com.gmail.doloiu22.repository.ParkingSessionRepository;
import com.gmail.doloiu22.repository.ParkingSpaceRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.Optional;

@Service
public class ParkingSessionService {

    @Autowired
    private ParkingSessionRepository parkingSessionRepository;
    @Autowired
    private ParkingSpaceRepository parkingSpaceRepository;

    public Optional<ParkingSessionEntity> findById(Long id) {
        return parkingSessionRepository.findById(id);
    }

    public List<ParkingSessionEntity> findAllBySessionID(Long sessionID) {
        return parkingSessionRepository.findAllBySessionID(sessionID);
    }

    public String getNameOfParkingSpaceInParkingSession(ParkingSessionEntity parkingSessionEntity) {
        Optional<ParkingSpaceEntity> parkingSpaceEntity = parkingSpaceRepository.findById(parkingSessionEntity.getParkingSpaceID());
        if (parkingSpaceEntity.isPresent())
            return parkingSpaceEntity.get().getName();
        else return "N/A";
    }
}
