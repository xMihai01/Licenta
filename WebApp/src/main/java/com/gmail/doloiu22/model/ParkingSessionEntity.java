package com.gmail.doloiu22.model;

import jakarta.persistence.*;
import lombok.Getter;
import lombok.Setter;
import org.springframework.cglib.core.Local;

import java.time.*;
import java.time.format.DateTimeFormatter;

@Getter @Setter
@Entity
@Table(name="parking_session")
public class ParkingSessionEntity {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Basic
    @Column(name = "session_id")
    private Long sessionID;

    @Basic
    @Column(name = "parking_camera_id")
    private Long parkingCameraID;

    @Basic
    @Column(name = "parking_space_id")
    private Long parkingSpaceID;

    @Basic
    @Column(name = "start_time")
    private LocalDateTime startTime;

    @Basic
    @Column(name = "end_time")
    private LocalDateTime endTime;

    public boolean didItExit() {
        return !endTime.isEqual(LocalDateTime.of(
                1970, 1, 1, 2, 0, 0, 0));
    }
    public String getStartTimeAsString() {
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm");
        return startTime.format(formatter);
    }
    public String getEndTimeAsString() {
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm");
        return endTime.format(formatter);
    }
    public String getTimeSpent() {
        if (didItExit())
            return Long.toString(Duration.between(startTime, endTime).toMinutes());

        LocalDateTime currentDateTime = LocalDateTime.now();
        long minutesDifference = Duration.between(startTime, currentDateTime).toMinutes();
        return Long.toString(minutesDifference);
    }
}
