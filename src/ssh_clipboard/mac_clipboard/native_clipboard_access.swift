import Cocoa

@_expose(Cxx)
public final class ClipboardMonitor {
    private var changeCount: Int = NSPasteboard.general.changeCount

    public init() {}

    public func paste() -> String {
        return NSPasteboard.general.string(forType: .string) ?? ""
    }

    public func copy(_ content: String) {
        NSPasteboard.general.clearContents()
        NSPasteboard.general.setString(content, forType: .string)
    }

    public func checkForChanges() -> Bool {
        guard NSPasteboard.general.changeCount != changeCount else { return false }
        changeCount = NSPasteboard.general.changeCount
        return true
    }
}
