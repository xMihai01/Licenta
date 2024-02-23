package com.gmail.doloiu22.model;

import jakarta.persistence.*;
import lombok.Getter;
import lombok.Setter;
import org.springframework.cglib.core.Local;

import java.time.*;
import java.time.format.DateTimeFormatter;

@Getter @Setter
@Entity
@Table(name="session")
public class SessionEntity {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Basic
    @Column(name = "entrance_time")
    private LocalDateTime entranceTime;

    @Basic
    @Column(name = "exit_time")
    private LocalDateTime exitTime;

    @Basic
    @Column(name = "license_plate")
    private String licensePlate;

    @Basic
    @Column(name = "secret_id")
    private String secretID;

    public boolean didItExit() {
        return !exitTime.isEqual(LocalDateTime.of(
                1970, 1, 1, 2, 0, 0, 0));
    }
    public String getEntranceTimeAsString() {
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm");
        return entranceTime.format(formatter);
    }
    public String getExitTimeAsString() {
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm");
        return exitTime.format(formatter);
    }
    public String getTimeSpent() {
        if (didItExit())
            return Long.toString(Duration.between(entranceTime, exitTime).toMinutes());

        LocalDateTime currentDateTime = LocalDateTime.now();
        long minutesDifference = Duration.between(entranceTime, currentDateTime).toMinutes();
        return Long.toString(minutesDifference);
    }
    public long getTimeSpentAsLong() {
        if (didItExit())
            return Duration.between(entranceTime, exitTime).toMinutes();

        LocalDateTime currentDateTime = LocalDateTime.now();
        return Duration.between(entranceTime, currentDateTime).toMinutes();
    }
}
