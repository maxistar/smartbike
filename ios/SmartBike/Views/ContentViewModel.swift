//
//  ContentViewModel.swift
//  SmartBike
//
//  Created by Maksim Starikov on 03.09.23.
//

import Foundation

final class ContentViewModel: ObservableObject {
    
    @Published var isLoading = true
    @Published var version: Int = 0
    @Published var timestamp: String = ""
    @Published var nextRequest: String = ""
    @Published var batteryInfo: BatteryInfo = BatteryInfo()
    
    
    func fetchData() {
        isLoading = true
        Task {
            let (data, _) = try await URLSession.shared.data(from: URL(string:"https://changeme/api/info/latest")!)
            
            let decodedResponse = try? JSONDecoder().decode(TrackingInfo.self, from: data)
            let trackInfo = decodedResponse?.battery_info ?? "error"
            version = decodedResponse?.version ?? 0
            let timestampString = decodedResponse?.timestamp ?? ""
            

            let batteryData: Data = trackInfo.data(using: .utf8)!
            batteryInfo = try! JSONDecoder().decode(BatteryInfo.self, from: batteryData)
            
            // Create a DateFormatter for UTC
            let utcFormatter = DateFormatter()
            utcFormatter.dateFormat = "yyyy-MM-dd'T'HH:mm:ss.SSS'Z'"
            utcFormatter.timeZone = TimeZone(identifier: "UTC")
            
            // Parse the UTC date string
            if let utcDate = utcFormatter.date(from: timestampString) {
                
                let sleepInSeconds = Double(batteryInfo.sleep) ?? 0
                let utcDateNext = utcDate.addingTimeInterval(sleepInSeconds)
                
                // Create a DateFormatter for Berlin timezone
                let berlinFormatter = DateFormatter()
                berlinFormatter.dateFormat = "yyyy-MM-dd HH:mm:ss"
                berlinFormatter.timeZone = TimeZone(identifier: "Europe/Berlin")
                
                // Format the date in Berlin timezone
                timestamp = berlinFormatter.string(from: utcDate)

                nextRequest = berlinFormatter.string(from: utcDateNext)
                
            } else {
                print("Failed to parse the date string.")
            }
            
            isLoading = false
        }
    }
}

struct TrackingInfo: Codable {
    var version: Int?
    var timestamp: String?
    var battery_info: String?
}

struct BatteryInfo: Codable {
    var latitude: String = ""
    var longitude: String = ""
    var battery: String = ""
    var solarBattery: String = ""
    var sleep: String = ""
    var execution: String = ""
}
