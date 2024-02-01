package com.gmail.doloiu22.model;

import jakarta.persistence.*;
import lombok.Getter;
import lombok.Setter;

@Getter @Setter
@Entity
@Table(name="reports")
public class ReportEntity {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Basic
    @Column(name="session_id")
    private Long sessionID;

    @Basic
    @Column(name = "reporter_license_plate")
    private String reporterLicensePlate;

    @Basic
    @Column(name = "suspect_license_plate")
    private String suspectLicensePlate;

    @Basic
    @Column(name = "time_period")
    private String timePeriod;

    @Basic
    @Column(name = "details")
    private String details;

    @Basic
    @Column(name = "contact")
    private String contact;

    @Basic
    @Column(name = "opened")
    private boolean opened;
}
