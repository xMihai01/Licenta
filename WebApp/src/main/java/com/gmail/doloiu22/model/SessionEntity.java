package com.gmail.doloiu22.model;

import jakarta.persistence.*;
import lombok.Getter;
import lombok.Setter;

import java.time.LocalDateTime;

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
}
